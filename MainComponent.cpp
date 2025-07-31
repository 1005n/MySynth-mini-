#include "MainComponent.h"

//正弦波を鳴らすための簡単なsound
      struct SineSound:public juce::SynthesiserSound//音の種類
{
	bool appliesToNote(int) override {return true;} //intはmidiノート番号
							//そもそもSynthesiserSoundクラスに"virtual bool appliesToNote(int MidiNoteNuumber) = 0"という「このSoundは指定されたノート番号に対応
							//していますか？」ときかれたらtrueかfalseを返す関数がある。それを常にtrueで返す様に上書き→全てのノートに対応する
	bool appliesToChannel(int) override {return true;} //上記と同様に「全てのMIDIチャンネルに対応するよ？」と言っている
};

//正弦波を生成する簡単なVoice
       struct SineVoice:public juce::SynthesiserVoice//実際に音を鳴らす処理（音の生成）
{
	bool canPlaySound(juce::SynthesiserSound* sound) override {return dynamic_cast<SineSound*> (sound) != nullptr;} //このSoundを鳴らすことができるか判定する関数
															//soundをSineSound型に変換することができたらtrueを返す
	void startNote (int midiNoteNumber,float velocity,juce::SynthesiserSound*,int) override //ノートオンされた時にこの音をどのくらいの高さ（Hz）でどの位の音量で鳴らすのかの準備する
												//関数のヘッダ部分

	{
		currentAngle = 0.0; //現在の波の角度を初期化→サイン波はsin(角度）で音を作るのでここからスタート
		level = velocity;  //鳴らす音の音量をセットする→鍵盤を押す強さをそのまま用いる
		tailOff = 0.0;    //音の余韻処理の初期化
				 //これらは変数に代入しているだけで宣言はprivateで行われている
		auto freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber); //midiノート番号を周波数に変換してくれる

		auto cyclesPerSample = freq /getSampleRate();  //getSampleRateは現在のサンプリングレート（Hz）を取得します
							       //周波数をサンプリングレートで割ると「１サンプルあたり何回波が進むのか」ということがわかる
		angleDelta =cyclesPerSample* 2.0* juce::MathConstants<double>::pi;//1サンプルあたりに進めるサイン波の角度→波形を書くときに使う(正直よくわかりきってない）
	}								 //周期をラジアンに変換する
										 //juce::MathConstants<double>::pi;はπを表す
	void stopNote(float,bool allowTailOff) override  //音を止めるための関数
							 //bool allowTailOffはtrue→減衰しながら音を消す
							 //　　    　　　　　false→即座に音を消す
	{
	if (allowTailOff)
	tailOff = 1.0; //renderNextblockで段々とこの数を減らしてフェードアウトさせる
        else
	clearCurrentNote(); //すなわちfalseなので即座に音を消す
	}
    

        void pitchWheelMoved(int) override{}//ピッチベンドホイールの処理（今回は使わない）
	void controllerMoved(int,int) override{}//midiコントロールチェンジの処理（今回は使わない）

        void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,int startSample,int numSamples) override//実際に音を出す処理の中核、毎サンプル音を出力する
	                                                                                                    //outputBuffer書き込む先のオーディオバッファ
													    //startSampleバッファのどこから書きはじめるか
                                                                                                            //numSample一回に出す音の長さ
	{
		if (angleDelta != 0.0)//0.0ときは鳴らす音がないのでこの処理はスキップされる
		{
			while(--numSamples >= 0)    //numSamplesは今回鳴らすサンプル数、１サンプルごとに音を出す処理を繰り返す処理
			{
				auto sample = (float) std::sin(currentAngle)*level;  //そもそもstd::sin()はc＋＋の標準ライブラリで、引数に応じてサイン波を返す
										     //levelを掛けることによって振幅を調整します
										     //floatにキャストします
				for (int channel = 0;channel < outputBuffer.getNumChannels();++channel) //outputBuffer~はjuceのAudioBufferクラスの関数で現在もってるチャンネル数を返します
				outputBuffer.addSample(channel,startSample,sample);                   //addSampleは一つのサンプル値をバッファに追加する関数
												      //左から、何チャンネル目に書き込むか、バッファのどの位置に書きこむか
												      //書き込む音の値となっている
				currentAngle += angleDelta; //angleDeltaの更新、サイン波の次の位置に進む処理
				++startSample;             //サンプル番号の更新、書き込み位置を次に進める
				}
		}
	}
	private: //SineVoice構造体のメンバ変数を定義している
		 //なぜpriveteなのか？→ppublicにするとどこでもこの変数を書き換えたり、宣言できてしまうから不正な値を入力してしまう可能性がある
		 double currentAngle = 0.0,angleDelta = 0.0,level = 0.0,tailOff = 0.0;
};

//===================================================================================================================================================
//MainComponentクラスの実装
MainComponent::MainComponent()//コンストラクタ
			      //コンストラクタの役割はオブジェクトを適切に初期化すること
			      //クラス定義外でのメンバ関数の定義　クラス名::メンバ関数名
{
	setSize(600,100); //ウィンドウサイズ
			  //継承親元にすでに宣言されているから値を代入するだけでいい

	synth.addVoice(new SineVoice()); //クラスメンバアクセス演算子“”.”（ドット演算子）
	synth.addSound(new SineSound()); //juceのsynthクラスのインスタンスにaddVoice,addSound関数（シンセに発音装置（Voice)を追加する関数）で自分が作ったやつを入れる
					 //ヒープ上でnewを使ってインスタンスがどうちゃらこうちゃらよくわかんない
					 //
	setAudioChannels(0,2);           //この関数はオーディオの入出力設定 
		                         //第一引数→入力チャンネル数(どこから入力するのか）今回は入力しないから０
					 //第二引数→出力チャンネル数（どこから出力するのか）　今回はステレオ出力だから２
}

        MainComponent::~MainComponent()   //コンストラクタで確保した記憶域の解放など→デストラクタ
        {
		shutdownAudio();
	}

//====================================================================================================================================================



        void MainComponent::prepareToPlay (int samplesPerBlockExpected,double sampleRate) //オーディオの再生準備するときに呼ばれる関数
											  //samplesPerBlockExpected→一度に渡されるオーディオバッファのサンプル数
											  //sampleRate→使用中の出力のサンプリングレート
	{
		synth.setCurrentPlaybackSampleRate(sampleRate);                           //synthはjuce::synthesiserのインスタンス
											  //synthesierは内部のVoiceに正しい周波数を計算させるためにサンプリングレートを設定させる必要がある
		synth.noteOn(1,60,0.8f);                                                  //(midiチャンネル番号,midiノート番号,ベロシティ（音の強さ）
	}


        void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) //音をバッファに書き込んで出力する
	{
		bufferToFill.clearActiveBufferRegion(); //バッファを初期化
		juce::MidiBuffer dummyMidi;             //空のmidiバッファを作成
							//今回はmidi入力を使わないから空のバッファを渡している
		synth.renderNextBlock(*bufferToFill.buffer,dummyMidi,bufferToFill.startSample,bufferToFill.numSamples);//midiノート情報におうじて音声baffaに音を生成する
														       //（出力先,空,書き込み開始位置,今回出力すべきサンプル数）
	}
       
        void MainComponent::releaseResources()
        {
		//リソースの開放（今回は何もしない）
	}



	
