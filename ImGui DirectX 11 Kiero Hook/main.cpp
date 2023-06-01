#include "includes.h"
#include "font.h"
#include "globals.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);

	ImGuiStyle* style = &ImGui::GetStyle();

	style->Alpha = 0.88f;
	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.99f, 0.99f, 0.99f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.40f);

	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);

	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 0.55f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 0.55f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 0.55f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.60f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 0.55f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 0.50f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);

	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 0.55f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.32f, 0.20f, 0.43f, 0.50f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.46f, 0.37f, 0.61f, 0.45f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);

	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);

	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.56f, 0.44f, 0.73f, 1.0f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	ImFontConfig font;
	font.FontDataOwnedByAtlas = false;

	io.Fonts->AddFontFromMemoryTTF((void*)rawData, sizeof(rawData), 18.5f, &font);  
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	if (GetAsyncKeyState(VK_INSERT) & 1) {
		globals::show = !globals::show;
	}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();
		auto start = std::chrono::system_clock::now();
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);

		ImGui::SetNextWindowPos({ 15, 15 });
		ImGui::SetNextWindowSize({ 370, 50 });

		ImGui::Begin("Watermark", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
		ImGui::Text("Hackytubbies v1  |  by Mau#7666");

		ImGui::End();

	if (globals::show)
	{
		ImGui::SetNextWindowSize(ImVec2(650, 350));
		ImGui::Begin(("Slendytubbies III  -  Cheat Menu"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		{
			ImGui::SameLine();
			if (ImGui::Button(("Main"), ImVec2(139.5, 30)))
			{
				globals::tab = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button(("Misc"), ImVec2(139.5, 30)))
			{
				globals::tab = 1;

			}
			ImGui::SameLine();
			if (ImGui::Button(("Weapons"), ImVec2(139.5, 30)))
			{
				globals::tab = 2;

			}
			ImGui::SameLine();
			if (ImGui::Button(("Options"), ImVec2(139.5, 30)))
			{
				globals::tab = 3;
			}
		}
		ImGui::Spacing();
		if (globals::tab == 0)
		{
			if (ImGui::InputFloat(("Velocity"), &globals::coins))
			{

			}
		}
		if (globals::tab == 1)
		{
			if (ImGui::Button("Self Destruct"))
			{
				kiero::shutdown();
			}

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("End to hook possiblement no reinjection.");
			}
		}
		if (globals::tab == 2)
		{
			ImGui::Text("Select a Weapon and its settings..");
			ImGui::Spacing();

			if (ImGui::Button("XIX"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Gun v1.");
			}

			ImGui::SameLine();
			if (ImGui::Button("XIX II"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Gun 2.");
			}

			ImGui::SameLine();
			if (ImGui::Button("Shorty"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 3.");
			}

			if (ImGui::Button("VZ61"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 4.");
			}

			ImGui::SameLine();
			if (ImGui::Button("44 Combat"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 5.");
			}

			ImGui::SameLine();
			if (ImGui::Button("MCS870"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 6.");
			}

			if (ImGui::Button("MP5N"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 7.");
			}

			ImGui::SameLine();
			if (ImGui::Button("M40A3"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 8.");
			}

			ImGui::SameLine();
			if (ImGui::Button("AKM"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 9.");
			}

			if (ImGui::Button("M249-Saw"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 10.");
			}

			ImGui::SameLine();
			if (ImGui::Button("RPG"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 11.");
			}

			ImGui::SameLine();
			if (ImGui::Button("MK 16"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 12.");
			}

			if (ImGui::Button("Knife"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 13.");
			}

			ImGui::SameLine();
			if (ImGui::Button("Machete"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 14.");
			}

			ImGui::SameLine();
			if (ImGui::Button("Fireaxe"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 15.");
			}

			if (ImGui::Button("Katana"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 16.");
			}

			ImGui::SameLine();
			if (ImGui::Button("Chainsaw"))
			{
				kiero::shutdown();
			}
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip("Get a Weapon 17.");
			}
		}
		if (globals::tab == 3)
		{
			ImGui::Text("Aqui podras aplicar algunos ajustes.");
		}
		ImGui::End();

	}

	ImGui::Render();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}