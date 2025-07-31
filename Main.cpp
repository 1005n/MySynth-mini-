#include <JuceHeader.h>
#include "MainComponent.h"

class MainWindow:public juce::DocumentWindow  //juceの基本的なGUIウィンドウクラス
{
	public:
		MainWindow()  //MainWindowのコンストラクタ、引数に（表示名,背景色,ボタンの種類）
			:juce::DocumentWindow("MySynth(mini)",juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),
					juce::DocumentWindow::allButtons)

                {
                setUsingNativeTitleBar(true);       //osが提供するネイティブなタイトルバーを使うかどうか
						    //true→wwindowsの標準的なタイトルバー
						    //false→jjuceが自前で用意するタイトルバー
		setContentOwned(new MainComponent(),true); //MainWindowに表示する中身のコンポーネントをセットしている
							  //動的に生成したMainComponentを表示内容として扱う
		centreWithSize(getWidth(),getHeight());   //ウィンドウ幅と高さを表している
		setVisible(true);                         //ウィンドウを表示するかどうか、true→表示する　false→表示しない
		}

		void closeButtonPressed() override        //ウィンドウの右上に表示された×が押されたときに呼ばれる処理
		{
			juce::JUCEApplication::getInstance() ->systemRequestedQuit();  //アプリケーションを終了してくださいとjuceに伝える
										       //アロー演算子　x->y  ｘが指すオブジェクトのメンバyをアクセスする
										       //よくわかりません
	        }

};


class MyApplication : public juce::JUCEApplication     //アプリケーション本体を定義するクラス
{
	public:
		const juce::String getApplicationName() override {return "MySynth(mini)app";}  //アプリケーションの名前を返す関数
		const juce::String getApplicationVersion() override {return "1.0";}            //アプリケーションのバージョンを返す関数

		void initialise(const juce::String&) override //ここでウィンドウを表示したり必要な初期化を行います、アプリが起動したときに一番最初に呼び出される関数
							      //引数は使っていない

		{
			mainWindow.reset(new MainWindow());  //よく分からない
		}

		void shutdown() override                     //juceアプリケーション終了時にmainWindowをnulptrにしてウィンドウを明示的に破棄します
		{
			mainWindow = nullptr;
		}
	private:
		std::unique_ptr<MainWindow> mainWindow; //スマートポインタの一種、MainWindowインスタンスを一個だけ作って使い終わったら消してねという意味
};
START_JUCE_APPLICATION(MyApplication)     //juceアプリケーションのエントリポイント（main関数）を自動生成します
