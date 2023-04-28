#include "Renderer.h"

#include "CleanedWindows.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include "GameSettings.h"
#include "WindowsException.h"
#include "WindowHandler.h"

#include "ColorMaterial.h"

using Microsoft::WRL::ComPtr;

#pragma region Pimpl

class Renderer::RendererImpl
{
public:
	explicit RendererImpl(HWND hwnd) noexcept;
	virtual ~RendererImpl() = default;

	RendererImpl(const RendererImpl& other) noexcept = delete;
	RendererImpl& operator=(const RendererImpl& other) noexcept = delete;
	RendererImpl(RendererImpl&& other) noexcept = delete;
	RendererImpl& operator=(RendererImpl&& other) noexcept = delete;

	virtual void* GetDevice() const = 0;
	virtual void* GetDeviceContext() const = 0;

	virtual void BeginFrame() const = 0;
	virtual void EndFrame() const = 0;

	virtual void RenderTestTriangle() = 0;

protected:
	/* DATA MEMBERS */

	HWND m_HWnd;

	inline static float m_DefaultBackgroundColor[4] = { .5f, .5f, .5f, 1.0f };
	inline static bool m_VSyncEnabled{ GameSettings::useVSync };
	
};

Renderer::RendererImpl::RendererImpl(HWND hwnd) noexcept
	: m_HWnd{ hwnd }
{
}

#pragma endregion

#pragma region DX11

class DirectX11 final : public Renderer::RendererImpl
{
public:
	explicit DirectX11(HWND hwnd);
	~DirectX11() override = default;

	DirectX11(const DirectX11& other) noexcept = delete;
	DirectX11& operator=(const DirectX11& other) noexcept = delete;
	DirectX11(DirectX11&& other) noexcept = delete;
	DirectX11& operator=(DirectX11&& other) noexcept = delete;

	void* GetDevice() const override;
	void* GetDeviceContext() const override;

	void BeginFrame() const override;
	void EndFrame() const override;

	void RenderTestTriangle() override;

private:
	/* DATA MEMBERS */

	ComPtr<ID3D11Device> m_pDevice{};
	ComPtr<IDXGISwapChain> m_pSwapChain{};
	ComPtr<ID3D11DeviceContext> m_pDeviceContext{};
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView{};

	/* PRIVATE METHODS */
	
};

DirectX11::DirectX11(HWND hwnd) : RendererImpl{ hwnd }
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_HWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	UINT flags{};
#ifdef _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	PGWND_THROW_IF_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_pSwapChain,
		&m_pDevice,
		nullptr,
		&m_pDeviceContext
	));

	ComPtr<ID3D11Resource> pBackBuffer{};
	PGWND_THROW_IF_FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	PGWND_THROW_IF_FAILED(m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pRenderTargetView));

	// Setup Output Merger
	m_pDeviceContext->OMSetRenderTargets(1u, m_pRenderTargetView.GetAddressOf(), nullptr);

	// Setup Viewport
	D3D11_VIEWPORT vp{};
	vp.Width = GameSettings::windowWidth;
	vp.Height = GameSettings::windowHeight;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1u, &vp);
}

void* DirectX11::GetDevice() const
{
	return m_pDevice.Get();
}

void* DirectX11::GetDeviceContext() const
{
	return m_pDeviceContext.Get();
}

void DirectX11::BeginFrame() const
{
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), m_DefaultBackgroundColor);
}

void DirectX11::EndFrame() const
{
	if (m_VSyncEnabled)
		PGWND_THROW_IF_FAILED(m_pSwapChain->Present(1u, 0u));
	else
		PGWND_THROW_IF_FAILED(m_pSwapChain->Present(0u, 0u));
}

void DirectX11::RenderTestTriangle()
{
	struct TestVertex
	{
		float x{}, y{};
	};

	// Vertex Buffer
	ComPtr<ID3D11Buffer> pVertexBuffer;

	static constexpr TestVertex vertices[] =
	{
		{ .0f, .5f},
		{ .5f, -.5f},
		{ -.5f, -.5f}
	};

	D3D11_BUFFER_DESC vbDesc{};
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.CPUAccessFlags = 0u;
	vbDesc.MiscFlags = 0u;
	vbDesc.ByteWidth = sizeof(vertices);
	vbDesc.StructureByteStride = sizeof(TestVertex);

	D3D11_SUBRESOURCE_DATA vsd{};
	vsd.pSysMem = vertices;

	PGWND_THROW_IF_FAILED(m_pDevice->CreateBuffer(&vbDesc, &vsd, &pVertexBuffer));

	constexpr UINT vbStride = sizeof(TestVertex);
	constexpr UINT vbOffset = 0u;
	m_pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &vbStride, &vbOffset);

	// Index Buffer
	ComPtr<ID3D11Buffer> pIndexBuffer;

	constexpr uint16_t indices[] =
	{
		0, 1, 2
	};

	D3D11_BUFFER_DESC ibDesc{};
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.CPUAccessFlags = 0u;
	ibDesc.MiscFlags = 0u;
	ibDesc.ByteWidth = sizeof(indices);
	ibDesc.StructureByteStride = sizeof(uint16_t);

	D3D11_SUBRESOURCE_DATA isd{};
	isd.pSysMem = indices;

	PGWND_THROW_IF_FAILED(m_pDevice->CreateBuffer(&ibDesc, &isd, &pIndexBuffer));

	m_pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	// IA
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Material
	const auto pMaterial = std::make_unique<ColorMaterial>();
	pMaterial->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	pMaterial->Bind();

	//m_pDeviceContext->Draw(UINT(std::size(vertices)), 0u);
	m_pDeviceContext->DrawIndexed(static_cast<UINT>(std::size(indices)), 0u, 0u);
}

#pragma endregion

#pragma region DX12

class DirectX12 final: public Renderer::RendererImpl
{
public:
	explicit DirectX12(HWND hWnd);
	~DirectX12() override;

	DirectX12(const DirectX12& other) = delete;
	DirectX12& operator=(const DirectX12& other) noexcept = delete;
	DirectX12(DirectX12&& other) = delete;
	DirectX12& operator=(DirectX12&& other) noexcept = delete;

	void* GetDevice() const override;
	void* GetDeviceContext() const override;

	void BeginFrame() const override;
	void EndFrame() const override;

	void RenderTestTriangle() override;

private:
	/* NESTED CLASSES */

	class DX12Command final
	{
	public:
		explicit DX12Command(ID3D12Device8* pDevice, D3D12_COMMAND_LIST_TYPE type);
		~DX12Command();

		DX12Command(const DX12Command& other) noexcept = delete;
		DX12Command& operator=(const DX12Command& other) noexcept = delete;
		DX12Command(DX12Command&& other) noexcept = delete;
		DX12Command& operator=(DX12Command&& other) noexcept = delete;

		void BeginFrame() const;
		void EndFrame();

		[[nodiscard]] ID3D12CommandQueue* GetCommandQueue() const { return m_pCommandQueue.Get(); }
		[[nodiscard]] ID3D12GraphicsCommandList6* GetCommandList() const { return m_pCommandList.Get(); }
		[[nodiscard]] UINT GetFrameIndex() const { return m_FrameIndex; }

	private:
		/* NESTED CLASSES */

		struct CommandFrame final
		{
			ComPtr<ID3D12CommandAllocator> m_pAllocator;
			UINT64 m_FenceValue{};

			void Wait(HANDLE fenceEvent, ID3D12Fence1* pFence) const;
		};

		/* DATA MEMBERS */

		ComPtr<ID3D12CommandQueue> m_pCommandQueue;
		ComPtr<ID3D12GraphicsCommandList6> m_pCommandList;
		inline static constexpr int s_BufferCount{ 3 };
		CommandFrame m_CommandFrames[s_BufferCount]{};
		UINT m_FrameIndex{};

		ComPtr<ID3D12Fence1> m_pFence;
		UINT64 m_FenceValue{};
		HANDLE m_FenceEvent{};

		/* PRIVATE METHODS */

	};

	/* DATA MEMBERS */

	ComPtr<ID3D12Device8> m_pDevice{};
	//Factory??
	std::unique_ptr<DX12Command> m_pCommand;

	/* PRIVATE METHODS */

	ComPtr<IDXGIAdapter4> FindBestAdapter(IDXGIFactory7* pDXGIFactory, D3D_FEATURE_LEVEL minFeatureLevel) const;
	D3D_FEATURE_LEVEL FindMaxFeatureLevel(IDXGIAdapter4* pAdapter) const;
	
};

DirectX12::DirectX12(HWND hWnd) : RendererImpl{ hWnd }
{
	ComPtr<IDXGIFactory7> pDXGIFactory;

	UINT DXGIFactoryFlags{};

#ifdef _DEBUG
	{
		ComPtr<ID3D12Debug3> pDebugInterface;
		PGWND_THROW_IF_FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugInterface)));
		pDebugInterface->EnableDebugLayer();

		DXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	}
#endif

	PGWND_THROW_IF_FAILED(CreateDXGIFactory2(DXGIFactoryFlags, IID_PPV_ARGS(&pDXGIFactory)));

	const ComPtr<IDXGIAdapter4> pDXGIAdapter = FindBestAdapter(pDXGIFactory.Get(), D3D_FEATURE_LEVEL_11_0);

	const D3D_FEATURE_LEVEL maxFeatureLevel = FindMaxFeatureLevel(pDXGIAdapter.Get());

	PGWND_THROW_IF_FAILED(D3D12CreateDevice(pDXGIAdapter.Get(), maxFeatureLevel, IID_PPV_ARGS(&m_pDevice)));

	m_pCommand = std::make_unique<DX12Command>(m_pDevice.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);

#ifdef _DEBUG
	{
		ComPtr<ID3D12InfoQueue> pInfoQueue;
		PGWND_THROW_IF_FAILED(m_pDevice->QueryInterface(IID_PPV_ARGS(&pInfoQueue)));
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
	}
#endif
}

DirectX12::~DirectX12()
{
#ifdef _DEBUG
	{
		ComPtr<ID3D12InfoQueue> pInfoQueue;
		m_pDevice->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, false);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, false);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, false);
	}
#endif
}


void* DirectX12::GetDevice() const
{
	return m_pDevice.Get();
}

void* DirectX12::GetDeviceContext() const
{
	return nullptr;
}

void DirectX12::BeginFrame() const
{
	m_pCommand->BeginFrame();
}

void DirectX12::EndFrame() const
{
	m_pCommand->EndFrame();
}

void DirectX12::RenderTestTriangle()
{
	
}

DirectX12::DX12Command::DX12Command(ID3D12Device8* pDevice, D3D12_COMMAND_LIST_TYPE type)
{
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Type = type;
	PGWND_THROW_IF_FAILED(pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_pCommandQueue)));

	for (int i{}; i < s_BufferCount; ++i)
		PGWND_THROW_IF_FAILED(pDevice->CreateCommandAllocator(type, IID_PPV_ARGS(&m_CommandFrames[i].m_pAllocator)));

	PGWND_THROW_IF_FAILED(pDevice->CreateCommandList(0, type, m_CommandFrames[0].m_pAllocator.Get(), nullptr, IID_PPV_ARGS(&m_pCommandList)));
	PGWND_THROW_IF_FAILED(m_pCommandList->Close());

	PGWND_THROW_IF_FAILED(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence)));
	m_FenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
}

DirectX12::DX12Command::~DX12Command()
{
	for (UINT i{}; i < s_BufferCount; ++i)
		m_CommandFrames[i].Wait(m_FenceEvent, m_pFence.Get());
	m_FrameIndex = 0;
	m_FenceValue = 0;
	if (m_FenceEvent)
	{
		CloseHandle(m_FenceEvent);
		m_FenceEvent = nullptr;
	}
		

}

void DirectX12::DX12Command::BeginFrame() const
{
	const auto& frame = m_CommandFrames[m_FrameIndex];
	frame.Wait(m_FenceEvent, m_pFence.Get());
	PGWND_THROW_IF_FAILED(frame.m_pAllocator->Reset());
	PGWND_THROW_IF_FAILED(m_pCommandList->Reset(frame.m_pAllocator.Get(), nullptr));
}

void DirectX12::DX12Command::EndFrame()
{
	PGWND_THROW_IF_FAILED(m_pCommandList->Close());
	ID3D12CommandList* const commandLists[]{ m_pCommandList.Get() };
	m_pCommandQueue->ExecuteCommandLists(_countof(commandLists), &commandLists[0]);

	++m_FenceValue;
	m_CommandFrames->m_FenceValue = m_FenceValue;

	m_pCommandQueue->Signal(m_pFence.Get(), m_FenceValue);

	m_FrameIndex = (m_FrameIndex + 1) % s_BufferCount;
}

void DirectX12::DX12Command::CommandFrame::Wait(HANDLE fenceEvent, ID3D12Fence1* pFence) const
{
	if (pFence->GetCompletedValue() < m_FenceValue)
	{
		PGWND_THROW_IF_FAILED(pFence->SetEventOnCompletion(m_FenceValue, fenceEvent));
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}

ComPtr<IDXGIAdapter4> DirectX12::FindBestAdapter(IDXGIFactory7* pDXGIFactory, D3D_FEATURE_LEVEL minFeatureLevel) const
{
	ComPtr<IDXGIAdapter4> pDXGIAdapter;

	// Iterate over adapters in descending order of performance
	for (UINT i{}; pDXGIFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&pDXGIAdapter)) != DXGI_ERROR_NOT_FOUND; ++i)
		// Take first adapter that supports min feature level
		if (SUCCEEDED(D3D12CreateDevice(pDXGIAdapter.Get(), minFeatureLevel, __uuidof(ID3D12Device), nullptr)))
			return pDXGIAdapter;

	return nullptr;
}

D3D_FEATURE_LEVEL DirectX12::FindMaxFeatureLevel(IDXGIAdapter4* pAdapter) const
{
	constexpr D3D_FEATURE_LEVEL featureLevels[]
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_2
	};

	D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelInfo{};
	featureLevelInfo.NumFeatureLevels = static_cast<UINT>(std::size(featureLevels));
	featureLevelInfo.pFeatureLevelsRequested = featureLevels;

	ComPtr<ID3D12Device> pDevice;
	PGWND_THROW_IF_FAILED(D3D12CreateDevice(pAdapter, featureLevels[0], IID_PPV_ARGS(&pDevice)));
	PGWND_THROW_IF_FAILED(pDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelInfo, sizeof(featureLevelInfo)));

	return featureLevelInfo.MaxSupportedFeatureLevel;
}

#pragma endregion

#pragma region Renderer

Renderer::~Renderer()
{
	delete m_pRendererImpl;
}

void* Renderer::GetDevice() const
{
	return m_pRendererImpl->GetDevice();
}

void* Renderer::GetDeviceContext() const
{
	return m_pRendererImpl->GetDeviceContext();
}

void Renderer::Init()
{
	switch (GameSettings::renderAPI)
	{
	case GameSettings::RenderAPI::DirectX11:
		m_pRendererImpl = new DirectX11{ WindowHandler::Get().GetHandle() };
		break;

	case GameSettings::RenderAPI::DirectX12:
		m_pRendererImpl = new DirectX12{ WindowHandler::Get().GetHandle() };
		break;
	}
	
}

void Renderer::BeginFrame() const
{
	m_pRendererImpl->BeginFrame();
}

void Renderer::EndFrame() const
{
	m_pRendererImpl->EndFrame();
}

void Renderer::RenderTestTriangle() const
{
	m_pRendererImpl->RenderTestTriangle();
}

#pragma endregion
