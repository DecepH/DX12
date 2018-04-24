#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dxgi.h>
#include <d3d12.h>

#if defined(_DEBUG)
#endif

DWORD WINAPI RenderThreadMain(LPVOID lpThreadParameter)
{
	HWND hWnd = static_cast<HWND>(lpThreadParameter);

	IDXGIFactory *pDXGIFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&pDXGIFactory));

	ID3D12Device *pD3D12Device = NULL;
	{
		IDXGIAdapter *pDXGIAdapter;
		for (UINT i = 0U; SUCCEEDED(pDXGIFactory->EnumAdapters(i, &pDXGIAdapter)); ++i)
		{
			if (SUCCEEDED(D3D12CreateDevice(pDXGIAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pD3D12Device))))
			{
				pDXGIAdapter->Release();
				break;
			}
			pDXGIAdapter->Release();
		}
	}

	ID3D12CommandQueue *pDirectCommandQueue;
	{
		D3D12_COMMAND_QUEUE_DESC cqdc;
		cqdc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		cqdc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		cqdc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		cqdc.NodeMask = 0x1;
		pD3D12Device->CreateCommandQueue(&cqdc, IID_PPV_ARGS(&pDirectCommandQueue));
	}

	IDXGISwapChain *pDXGISwapChain;
	{
		DXGI_SWAP_CHAIN_DESC scdc;
		scdc.BufferDesc.Width = 0U;
		scdc.BufferDesc.Height = 0U;
		scdc.BufferDesc.RefreshRate.Numerator = 60U;
		scdc.BufferDesc.RefreshRate.Denominator = 1U;
		scdc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scdc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scdc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scdc.SampleDesc.Count = 1U;
		scdc.SampleDesc.Quality = 0U;
		scdc.BufferUsage = DXGI_USAGE_SHADER_INPUT;
		scdc.BufferCount = 2;
		scdc.OutputWindow = hWnd;
		scdc.Windowed = TRUE;
		scdc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		scdc.Flags = 0U;
		pDXGIFactory->CreateSwapChain(pDirectCommandQueue, &scdc, &pDXGISwapChain);
	}
	pDXGIFactory->Release();

	pDXGISwapChain->Present(0, 0);

	return 0U;
}

