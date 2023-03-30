#include "Renderer.h"

#include "CleanedWindows.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "WindowsException.h"
#include "WindowHandler.h"

using Microsoft::WRL::ComPtr;

class Renderer::RendererImpl abstract
{
public:
	explicit RendererImpl(HWND hwnd) noexcept;
	virtual ~RendererImpl() = default;

	RendererImpl(const RendererImpl& other) noexcept = delete;
	RendererImpl& operator=(const RendererImpl& other) noexcept = delete;
	RendererImpl(RendererImpl&& other) noexcept = delete;
	RendererImpl& operator=(RendererImpl&& other) noexcept = delete;

	virtual void BeginFrame() const = 0;
	virtual void EndFrame() const = 0;

	virtual void RenderTestTriangle() = 0;

protected:
	/* DATA MEMBERS */

	HWND m_HWnd;

	inline static float m_DefaultBackgroundColor[4] = { .5f, .5f, .5f, 1.0f };
	inline static bool m_VSyncEnabled{ GameSettings::useVSync };

private:
	/* DATA MEMBERS */

	/* PRIVATE METHODS */
	
};

class DirectX11 final : public Renderer::RendererImpl
{
public:
	explicit DirectX11(HWND hwnd);
	~DirectX11() override = default;

	DirectX11(const DirectX11& other) noexcept = delete;
	DirectX11& operator=(const DirectX11& other) noexcept = delete;
	DirectX11(DirectX11&& other) noexcept = delete;
	DirectX11& operator=(DirectX11&& other) noexcept = delete;

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

Renderer::RendererImpl::RendererImpl(HWND hwnd) noexcept
	: m_HWnd{ hwnd }
{
}

DirectX11::DirectX11(HWND hwnd) : Renderer::RendererImpl{ hwnd }
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
		unsigned char r{}, g{}, b{}, a{};
	};

	constexpr TestVertex vertices[] =
	{
		{ .0f, .5f, 255, 0, 0, 255 },
		{ .5f, -.5f, 0, 255, 0, 255 },
		{ -.5f, -.5f, 0, 0, 255, 255 }
	};

	ComPtr<ID3D11Buffer> pVertexBuffer;

	D3D11_BUFFER_DESC bDesc{};
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.Usage = D3D11_USAGE_DEFAULT;
	bDesc.CPUAccessFlags = 0u;
	bDesc.MiscFlags = 0u;
	bDesc.ByteWidth = sizeof(vertices);
	bDesc.StructureByteStride = sizeof(TestVertex);

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertices;

	PGWND_THROW_IF_FAILED(m_pDevice->CreateBuffer(&bDesc, &sd, &pVertexBuffer));

	constexpr UINT stride = sizeof(TestVertex);
	constexpr UINT offset = 0u;
	m_pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// PS
	ComPtr<ID3D11PixelShader> pPixelShader;
	ComPtr<ID3DBlob> pBlob;
	PGWND_THROW_IF_FAILED(D3DReadFileToBlob(L"../Engine/Shaders/TestPixelShader.cso", &pBlob));
	PGWND_THROW_IF_FAILED(m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	m_pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// VS
	ComPtr<ID3D11VertexShader> pVertexShader;
	PGWND_THROW_IF_FAILED(D3DReadFileToBlob(L"../Engine/Shaders/TestVertexShader.cso", &pBlob));
	PGWND_THROW_IF_FAILED(m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	m_pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	// IA
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	PGWND_THROW_IF_FAILED(m_pDevice->CreateInputLayout(ied, UINT(std::size(ied)), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));

	m_pDeviceContext->IASetInputLayout(pInputLayout.Get());

	m_pDeviceContext->Draw(UINT(std::size(vertices)), 0u);
}

Renderer::~Renderer()
{
	delete m_pRendererImpl;
}

void Renderer::Init()
{
	m_pRendererImpl = new DirectX11{ WindowHandler::Get().GetHandle() };
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
