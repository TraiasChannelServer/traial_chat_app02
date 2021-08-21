// DxLib_template
//

#include "framework.h"
#include "DxLib_template.h"
#include "DxLib.h"

typedef long long LL;
typedef std::pair<int, int> pairI2;
typedef std::pair<LL, LL> pairLL2;
typedef std::vector<int> VI;
typedef std::vector<LL> VLL;
typedef std::vector<VI> VVI;
typedef std::vector<VVI> VVVI;
typedef std::vector<std::string> VS;
#define shig_for(i, a, b) for(int i = (a); i < (b); ++i)
#define shig_rep(i, n) shig_for(i, 0, n)
#define shig_forB(bit,a,b) for(int bit = (a); bit < (1<<(b)); ++bit)
#define shig_repB(bit,n) shig_forB(bit,0,n)
#define tmp int temp = 0
#define str string s


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	SetMainWindowText(_T("traial_chat_app02"));
	ChangeWindowMode(TRUE);
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_MAXIMUM);
	SetFullScreenScalingMode(DX_FSSCALINGMODE_BILINEAR);
	SetGraphMode(640, 360, 32);
	SetWindowSizeChangeEnableFlag(TRUE);
	SetAlwaysRunFlag(TRUE);



	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	int mode = -1;
	while (!ProcessMessage()) {

		DrawString(0, 0, _T("select mode\nuser : put U key\nserver : put S key\nexit : put ESC key"), GetColor(255, 255, 255));

		if (CheckHitKey(KEY_INPUT_U))mode = 1;
		else if (CheckHitKey(KEY_INPUT_S))mode = 2;
		else if (CheckHitKey(KEY_INPUT_ESCAPE))mode = 3;
		else mode = -1;

		if (mode != -1) {
			ClearDrawScreen();
			break;
		}

	}

	if (mode == 3) {
		DrawString(0, 0, _T("put any key to exit"), GetColor(255, 255, 255));
	}
	else if (mode == 1) {

		TCHAR StrBuf[256];    // データバッファ
		IPDATA Ip;        // 接続用ＩＰアドレスデータ
		Ip.d1 = (unsigned char)192;
		Ip.d2 = (unsigned char)168;
		Ip.d3 = (unsigned char)5;
		Ip.d4 = (unsigned char)227;
		char username[256];
		int NetHandle = -1;        // ネットワークハンドル
		int DataLength = 0;        // 受信データ量保存用変数
		int port = 9850;

		int ip1 = 0, ip2 = 0, ip3 = 0, ip4 = 0;
		const char* fname = "config_u.txt";
		FILE* fp;
		auto em = fopen_s(&fp, fname, "r");
		auto at = fscanf_s(fp, "%d %d %d %d", &ip1, &ip2, &ip3, &ip4);
		auto au = fscanf_s(fp, "%d", &port);

		int ns = 0;
		auto nl = fscanf_s(fp, "%d", &ns);
		auto an = fscanf_s(fp, "%s", username, 256);
		fclose(fp);

		// ＩＰアドレスを設定
		//
		Ip.d1 = (unsigned char)ip1;
		Ip.d2 = (unsigned char)ip2;
		Ip.d3 = (unsigned char)ip3;
		Ip.d4 = (unsigned char)ip4;


		unsigned int Color = GetColor(255, 255, 255);
		DrawFormatString(0, 0, Color, _T("setting:\nIP number is %d.%d.%d.%d\nport number is %d"), ip1, ip2, ip3, ip4, port);
		DrawFormatString(0, 60, Color, _T("user name is %s"), username);
		DrawFormatString(0, 80, Color, _T("put any key to starting connecting"));
		WaitKey();

		// 通信を確立
		NetHandle = ConnectNetWork(Ip, port);

		// 確立が成功した場合のみ中の処理をする
		if (NetHandle != -1)
		{
			// データ送信



			NetWorkSend(NetHandle, "get_recieve", 12);
			while (!ProcessMessage())
			{
				// 取得していない受信データ量を得る
				DataLength = GetNetWorkDataLength(NetHandle);

				// 取得してない受信データ量が０じゃない場合はループを抜ける
				if (DataLength != 0) break;
			}
			NetWorkRecv(NetHandle, StrBuf, DataLength);
			DrawString(0, 120, StrBuf, GetColor(255, 255, 255));



			std::string st = std::string(username);

			NetWorkSend(NetHandle, st.c_str(), st.size() + 2);

			// データがくるのを待つ
			while (!ProcessMessage())
			{
				// 取得していない受信データ量を得る
				DataLength = GetNetWorkDataLength(NetHandle);

				// 取得してない受信データ量が０じゃない場合はループを抜ける
				if (DataLength != 0) break;
			}

			// データ受信
				// データをバッファに取得
			NetWorkRecv(NetHandle, StrBuf, DataLength);
			// 受信したデータを描

			DrawString(0, 140, StrBuf, GetColor(255, 255, 255));


			DrawString(0, 160, "put any ket to disconnect", GetColor(255, 255, 255));
			// キー入力待ち
			WaitKey();

			// 接続を断つ
			CloseNetWork(NetHandle);

			DrawString(0, 200, "disconnected. put some key or click to finish this program", GetColor(255, 255, 255));

			WaitKey();

		}
		else {
			DrawString(0, 100, _T("failed to connect..."), GetColor(255, 255, 255));
			WaitKey();
		}




	}
	else if (mode == 2) {

		TCHAR StrBuf[256];    // データバッファ
		IPDATA Ip;        // 接続用ＩＰアドレスデータ
		Ip.d1 = 192;
		Ip.d2 = 168;
		Ip.d3 = 5;
		Ip.d4 = 227;
		int NetHandle, LostHandle;        // ネットワークハンドル
		int DataLength = 0;        // 受信データ量保存用変数
		int port = 9850;
		char username[256];
		
		const char* fname = "config_s.txt";
		FILE* fp;
		auto em = fopen_s(&fp, fname, "r");
		auto au = fscanf_s(fp, "%d", &port);

		int ns = 0;
		//auto nl = fscanf_s(fp, "%d", &ns);
		auto an = fscanf_s(fp, "%s", username, 256);
		fclose(fp);



		// 接続してくるのを待つ状態にする
		PreparationListenNetWork(port);

		// 接続してくるかＥＳＣキーが押されるまでループ
		NetHandle = -1;
		while (!ProcessMessage() && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
		{

			std::string un = "username is " + std::string(username);
			DrawString(0, 0, _T(un.c_str()), GetColor(255, 255, 255));
			DrawString(0, 20, _T("please wait a connection, or push ESC key to exit this program."), GetColor(255, 255, 255));
			// 新しい接続があったらそのネットワークハンドルを得る
			NetHandle = GetNewAcceptNetWork();
			if (NetHandle != -1) break;
		}

		// 接続されていたら次に進む
		if (NetHandle != -1)
		{
			// 接続の受付を終了する
			StopListenNetWork();

			// 接続してきたマシンのＩＰアドレスを得る
			GetNetWorkIP(NetHandle, &Ip);

			// データが送られて来るまで待つ
			while (!ProcessMessage())
			{
				// 取得していない受信データ量が０以外のときはループから抜ける
				if (GetNetWorkDataLength(NetHandle) != 0) break;
			}

			// データ受信
			DataLength = GetNetWorkDataLength(NetHandle);    // データの量を取得
			NetWorkRecv(NetHandle, StrBuf, DataLength);    // データをバッファに取得

			// 受信したデータを描画
			DrawString(0, 60, StrBuf, GetColor(255, 255, 255));

			// 受信成功のデータを送信

			std::string st = "succese to connected!! thank you~";

			NetWorkSend(NetHandle, st.c_str(), st.size() + 2);

			while (!ProcessMessage())
			{
				// 取得していない受信データ量が０以外のときはループから抜ける
				if (GetNetWorkDataLength(NetHandle) != 0) break;
			}

			// データ受信
			DataLength = GetNetWorkDataLength(NetHandle);    // データの量を取得
			NetWorkRecv(NetHandle, StrBuf, DataLength);    // データをバッファに取得

			// 受信したデータを描画
			DrawString(0, 80, StrBuf, GetColor(255, 255, 255));


			std::string stt = std::string(username);

			NetWorkSend(NetHandle, stt.c_str(), stt.size() + 2);


			// 相手が通信を切断するまで待つ
			while (!ProcessMessage())
			{
				// 新たに切断されたネットワークハンドルを得る
				LostHandle = GetLostNetWork();

				// 切断された接続が今まで通信してた相手だった場合ループを抜ける
				if (LostHandle == NetHandle) break;
			}

			// 切断確認表示
			DrawString(0, 120, _T("disconnected"), GetColor(255, 255, 255));

			// キー入力待ち
			WaitKey();
		}
	


	}
	

	WaitKey();					// キーの入力待ち((7-3)『WaitKey』を使用)

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}