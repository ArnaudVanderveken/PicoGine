#include "Renderer.h"

#include "CleanedWindows.h"
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
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

protected:
	/* DATA MEMBERS */
	HWND m_HWnd;

private:
	/* DATA MEMBERS */

	/* PRIVATE METHODS */
	
};

class Renderer::DirectX11 final : public RendererImpl
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

Renderer::DirectX11::DirectX11(HWND hwnd)
	: RendererImpl{ hwnd }
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
}

void Renderer::DirectX11::BeginFrame() const
{
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), m_DefaultBackgroundColor);
}

void Renderer::DirectX11::EndFrame() const
{
	if (m_VSyncEnabled)
		PGWND_THROW_IF_FAILED(m_pSwapChain->Present(1u, 0u));
	else
		PGWND_THROW_IF_FAILED(m_pSwapChain->Present(0u, 0u));
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

void Renderer::RenderTestTriangle()
{
}
