#include "MainComponent.h"

//正弦波を鳴らすための簡単なsound
      struct SineSound:public juce::SynthesiserSound//音の種類
{
	bool appliesToNote(int) override {return true;} //intはmidiノート番号
							//そもそもSynthesiserSoundクラスに"virtual bool appliesToNote(int MidiNoteNuumber) = 0"という「このSoundは指定されたノート番号に対応
							//していますか？」ときかれたらtrueかfalseを返す関数がある。それを常にtrueで返す様に上書き→全てのノートに対応する
	bool appliesToChannel(int) override {return true;} //上記と同様に「全てのMIDIチャンネルに対応するよ？」と言っている
};
