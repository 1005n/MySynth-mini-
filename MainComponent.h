#pragma once//このファイルは一度しか読み込まないでねという意味
#include <JuceHeader.h>

class MainComponent/*子クラス*/:public juce::AudioAppComponent//親クラス
{
	public://クラス外からアクセスできる
	MainComponent();
	~MainComponent() override;

	//JUCEで音を扱う時に必要な特別な関数（オーバーライド関数）

	void prepareToPlay(int,double) override;//バッファサイズやサンプルレートの受け取り
	void getNextAudioBlock(const juce::AudioSourceChannelInfo&) override;
	//次に出す音の塊をここで作る、引数の中はここに音を書いてねという場所
	void releaseResources() override;//オーディオの処理が終わるときに呼ばれる関数です

	void resized() override {} //JUCEのGUIコンポーネントが変更されたときに呼ばれる関数を上書きしている
        
        private://クラス外からアクセスできない
	juce::Synthesiser synth;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)//コピー禁止とメモリリーク検出
								   //なぜコピー禁止か？→不正動作やクラッシュの原因
								   //メモリリークとは？→使い終わったメモリを解放し忘れて無駄にメモリを消費し続けるバグ
								   //メモリリークが引き起こす問題→まず、解放されないメモリがどんどんたまる
								   //　　　　　　　　　　　　　　→アプリの動作自体が重くなる
								   //　　　　　　　　　　　　　　→メモリ不足でアプリが落ちる
};
