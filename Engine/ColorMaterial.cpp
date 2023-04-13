#include "ColorMaterial.h"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <DirectXMath.h>

#include "Renderer.h"
#include "WindowsException.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

class ColorMaterial::ColorMaterialImpl
{
public:
	ColorMaterialImpl() noexcept = default;
	virtual ~ColorMaterialImpl() = default;

	ColorMaterialImpl(const ColorMaterialImpl& other) noexcept = delete;
	ColorMaterialImpl& operator=(const ColorMaterialImpl& other) noexcept = delete;
	ColorMaterialImpl(ColorMaterialImpl&& other) noexcept = delete;
	ColorMaterialImpl& operator=(ColorMaterialImpl&& other) noexcept = delete;

	virtual void Bind() = 0;

private:
	/* DATA MEMBERS */

	/* PRIVATE METHODS */
	
};

class DX11ColorMaterial final : public ColorMaterial::ColorMaterialImpl
{
public:
	DX11ColorMaterial() noexcept;
	~DX11ColorMaterial() override = default;

	DX11ColorMaterial(const DX11ColorMaterial& other) = delete;
	DX11ColorMaterial& operator=(const DX11ColorMaterial& other) noexcept = delete;
	DX11ColorMaterial(DX11ColorMaterial&& other) = delete;
	DX11ColorMaterial& operator=(DX11ColorMaterial&& other) noexcept = delete;

	void Bind() override;
	void SetColor(const XMFLOAT4& color);
	void SetColor(float r, float g, float b, float a = 1);

private:
	/* STRUCTS */

	struct PSConstantBufferData
	{
		XMFLOAT4 color{};
	};

	/* DATA MEMBERS */

	PSConstantBufferData m_PSConstantBufferData{};

	ComPtr<ID3D11Buffer> m_pPSConstantBuffer;
	ComPtr<ID3D11PixelShader> m_pPixelShader;
	ComPtr<ID3D11VertexShader> m_pVertexShader;
	ComPtr<ID3D11InputLayout> m_pInputLayout;

	/* PRIVATE METHODS */

	void UpdateBuffer() const;
};

DX11ColorMaterial::DX11ColorMaterial() noexcept
{
	const auto device = static_cast<ID3D11Device*>(Renderer::Get().GetDevice());

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(PSConstantBufferData);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA cbData;
	cbData.pSysMem = &m_PSConstantBufferData;
	cbData.SysMemPitch = 0;
	cbData.SysMemSlicePitch = 0;

	device->CreateBuffer(&cbDesc, &cbData, &m_pPSConstantBuffer);

	// PS
	ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"../Engine/Shaders/ColorPS.cso", &pBlob);
	device->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader);

	// VS
	D3DReadFileToBlob(L"../Engine/Shaders/ColorVS.cso", &pBlob);
	device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pVertexShader);

	// Input Layout
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	device->CreateInputLayout(ied, UINT(std::size(ied)), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pInputLayout);
}

void DX11ColorMaterial::Bind()
{
	const auto deviceContext = static_cast<ID3D11DeviceContext*>(Renderer::Get().GetDeviceContext());
	deviceContext->PSSetConstantBuffers(0, 1, m_pPSConstantBuffer.GetAddressOf());
	deviceContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
	deviceContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
	deviceContext->IASetInputLayout(m_pInputLayout.Get());
}

void DX11ColorMaterial::SetColor(const XMFLOAT4& color)
{
	m_PSConstantBufferData.color = color;

	UpdateBuffer();
}

void DX11ColorMaterial::SetColor(float r, float g, float b, float a)
{
	m_PSConstantBufferData.color.x = r;
	m_PSConstantBufferData.color.y = g;
	m_PSConstantBufferData.color.z = b;
	m_PSConstantBufferData.color.w = a;

	UpdateBuffer();
}

void DX11ColorMaterial::UpdateBuffer() const
{
	const auto deviceContext = static_cast<ID3D11DeviceContext*>(Renderer::Get().GetDeviceContext());

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	deviceContext->Map(m_pPSConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	const auto data = static_cast<float*>(mappedResource.pData);
	memcpy(data, &m_PSConstantBufferData, sizeof(PSConstantBufferData));

	deviceContext->Unmap(m_pPSConstantBuffer.Get(), 0);
}
