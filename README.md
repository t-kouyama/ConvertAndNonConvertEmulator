Convert And NonConvert Emulator
====

Alt などの特殊キーを無変換/変換キーとしても使えるようにするソフトウェア


## Description

英語キーボードのような無変換/変換キーが存在しない環境でも無変換/変換キーの機能を利用できるようにします。

左右の特殊キーを普通に押すと無変換/変換キーとして動作し、長押しまたは他のキーと組み合わせると元の特殊キーとして動作するようになります。


## Usage

ConvertAndNonConvertEmulator.exe を起動すると常駐し、それ以後の特殊キーの操作が置き換えられます。  
ウィンドウやアイコンは一切表示されないので、動作状況の確認や終了はタスクマネージャーなどから行ってください。

ConvertAndNonConvertEmulator.exe と同じフォルダの ConvertAndNonConvertEmulator.ini で次の設定ができます。

* `NonConvertKey` : 無変換キーとして使用するキー  
  164 : 左Alt &emsp; 91 : 左Windows &emsp; 162 : 左Ctrl &emsp; 20 : CapsLock &emsp; 0 : 使用しない

* `ConvertKey` : 変換キーとして使用するキー  
  165 : 右Alt &emsp; 92 : 右Windows &emsp; 163 : 右Ctrl &emsp; 93 : アプリケーション &emsp; 0 : 使用しない

* `LongPressDelay` : 長押しとして認識するまでの時間（ミリ秒）

* `FixHastyTyping` : 特殊キーを押して長押しとして認識される前に A～Z を押したときの動作  
  0 : 特殊キー + A～Z &emsp; 1 : 無変換/変換を押した後に A～Z  
  無変換/変換を押してすぐに入力を始めるような利用方法の際に入力ミスを軽減するための機能です。

設定は起動時に読み込まれます。起動中の場合は設定を反映させるために一度終了させる必要があります。

管理者権限を持つソフトウェアに対して機能させるには、ConvertAndNonConvertEmulator.exe も管理者権限で起動させてください。


## Requirements

* Windows 10

非常に単純なプログラムなので Windows 2000 以降であれば動作するのではないかと思いますが、Windows 10 以外ではテストしていません。


## License

このソフトウェアは、[MIT License](https://github.com/t-kouyama/ConvertAndNonConvertEmulator/blob/master/LICENSE) のもとで公開されています。
