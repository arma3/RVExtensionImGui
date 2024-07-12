#include "CtxBackup.hpp"
#include <d3d11.h>
#pragma comment( lib, "d3d11.lib" )       // direct3D library
//--------------------------------------------------------
// Either set these manually (based on how your renderer works) or use the D3D11 limits
//--------------------------------------------------------
#define MAX_VERTEX_BUFFERS          D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT
#define MAX_SHADER_SRV              D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT
#define MAX_SHADER_CONSTANT_BUFFERS D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT
#define MAX_SHADER_SAMPLERS         D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT
//--------------------------------------------------------
// This is mostly identital to how ImGui backs up and restores the context
//--------------------------------------------------------
struct BACKUP_DX11_STATE
{
	UINT                        ScissorRectsCount, ViewportsCount;
	D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	ID3D11RasterizerState*      RS;
	ID3D11BlendState*           BlendState;
	FLOAT                       BlendFactor[4];
	UINT                        SampleMask;
	UINT                        StencilRef;
	ID3D11DepthStencilState*    DepthStencilState;
	ID3D11ShaderResourceView*   VSShaderResources[MAX_SHADER_SRV];
	ID3D11ShaderResourceView*   PSShaderResources[MAX_SHADER_SRV];
	ID3D11SamplerState*         VSSamplers[MAX_SHADER_SAMPLERS];
	ID3D11SamplerState*         PSSamplers[MAX_SHADER_SAMPLERS];
	ID3D11PixelShader*          PS;
	ID3D11VertexShader*         VS;
	ID3D11GeometryShader*       GS;
	UINT                        PSInstancesCount, VSInstancesCount, GSInstancesCount;
	ID3D11ClassInstance         *PSInstances[256], *VSInstances[256], *GSInstances[256];   // 256 is max according to PSSetShader documentation
	D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
	ID3D11Buffer*               IndexBuffer;
	ID3D11Buffer*               VertexBuffers[MAX_VERTEX_BUFFERS];
	ID3D11Buffer*               VSConstBuffers[MAX_SHADER_CONSTANT_BUFFERS];
	ID3D11Buffer*               PSConstBuffers[MAX_SHADER_CONSTANT_BUFFERS];
	UINT                        IndexBufferOffset;
	UINT                        VertexBufferStrides[MAX_VERTEX_BUFFERS], VertexBufferOffsets[MAX_VERTEX_BUFFERS];
	DXGI_FORMAT                 IndexBufferFormat;
	ID3D11InputLayout*          InputLayout;
};

static BACKUP_DX11_STATE old = {};
//--------------------------------------------------------

DX11::ContextBackup::ContextBackup(ID3D11DeviceContext* c) : ctx{ c }
{
	if (ctx)
		Backup(ctx);
}
//--------------------------------------------------------
DX11::ContextBackup::~ContextBackup()
{
	if (ctx)
		Restore(ctx);
}
//--------------------------------------------------------
void DX11::ContextBackup::Backup(ID3D11DeviceContext* ctx)
{
	old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	ctx->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
	ctx->RSGetViewports(&old.ViewportsCount, old.Viewports);
	ctx->RSGetState(&old.RS);
	ctx->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
	ctx->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
	ctx->VSGetShaderResources(0, MAX_SHADER_SRV, old.VSShaderResources);
	ctx->PSGetShaderResources(0, MAX_SHADER_SRV, old.PSShaderResources);
	ctx->VSGetSamplers(0, MAX_SHADER_SAMPLERS, old.VSSamplers);
	ctx->PSGetSamplers(0, MAX_SHADER_SAMPLERS, old.PSSamplers);
	old.PSInstancesCount = old.VSInstancesCount = old.GSInstancesCount = 256;
	ctx->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
	ctx->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
	ctx->VSGetConstantBuffers(0, MAX_SHADER_CONSTANT_BUFFERS, old.VSConstBuffers);
	ctx->PSGetConstantBuffers(0, MAX_SHADER_CONSTANT_BUFFERS, old.PSConstBuffers);
	ctx->GSGetShader(&old.GS, old.GSInstances, &old.GSInstancesCount);

	ctx->IAGetPrimitiveTopology(&old.PrimitiveTopology);
	ctx->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
	ctx->IAGetVertexBuffers(0, MAX_VERTEX_BUFFERS, old.VertexBuffers, old.VertexBufferStrides, old.VertexBufferOffsets);
	ctx->IAGetInputLayout(&old.InputLayout);
}
//--------------------------------------------------------
void DX11::ContextBackup::Restore(ID3D11DeviceContext* ctx)
{
	// Restore modified DX state
	ctx->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
	ctx->RSSetViewports(old.ViewportsCount, old.Viewports);
	ctx->RSSetState(old.RS); if (old.RS) old.RS->Release();
	ctx->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) old.BlendState->Release();
	ctx->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) old.DepthStencilState->Release();
	ctx->VSSetShaderResources(0, MAX_SHADER_SRV, old.VSShaderResources);
	ctx->PSSetShaderResources(0, MAX_SHADER_SRV, old.PSShaderResources);
	for (UINT i = 0; i < MAX_SHADER_SRV; i++)
	{
		if (old.VSShaderResources[i]) old.VSShaderResources[i]->Release();
		if (old.PSShaderResources[i]) old.PSShaderResources[i]->Release();
	}
	ctx->VSSetSamplers(0, MAX_SHADER_SAMPLERS, old.VSSamplers);
	ctx->PSSetSamplers(0, MAX_SHADER_SAMPLERS, old.PSSamplers);
	for (UINT i = 0; i < MAX_SHADER_SAMPLERS; i++)
	{
		if (old.VSSamplers[i]) old.VSSamplers[i]->Release();
		if (old.PSSamplers[i]) old.PSSamplers[i]->Release();
	}
	ctx->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
	for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
	ctx->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
	ctx->VSSetConstantBuffers(0, MAX_SHADER_CONSTANT_BUFFERS, old.VSConstBuffers);
	ctx->PSSetConstantBuffers(0, MAX_SHADER_CONSTANT_BUFFERS, old.PSConstBuffers);
	for (int i = 0; i < MAX_SHADER_CONSTANT_BUFFERS; ++i)
	{
		if (old.VSConstBuffers[i]) old.VSConstBuffers[i]->Release();
		if (old.PSConstBuffers[i]) old.PSConstBuffers[i]->Release();
	}
	ctx->GSSetShader(old.GS, old.GSInstances, old.GSInstancesCount); if (old.GS) old.GS->Release();
	for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
	ctx->IASetPrimitiveTopology(old.PrimitiveTopology);
	ctx->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
	ctx->IASetVertexBuffers(0, MAX_VERTEX_BUFFERS, old.VertexBuffers, old.VertexBufferStrides, old.VertexBufferOffsets);
	for (UINT i = 0; i < MAX_VERTEX_BUFFERS; i++) if (old.VertexBuffers[i]) old.VertexBuffers[i]->Release();
	ctx->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();
}
