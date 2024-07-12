#pragma once
struct ID3D11DeviceContext;
namespace DX11
{
	class ContextBackup
	{
		ID3D11DeviceContext* ctx;
	public:
		ContextBackup(ID3D11DeviceContext* ctx);
		~ContextBackup();
	protected:
		static void Backup(ID3D11DeviceContext* ctx);
		static void Restore(ID3D11DeviceContext* ctx);
	};
}