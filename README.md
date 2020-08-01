Convert And NonConvert Emulator
====

キーボードの左右 Alt キーを無変換/変換キーとしても使えるようにするソフトウェア


## Description

英語キーボード上で日本語キーボードのような無変換/変換キーの機能を利用できるようにします。

* 左 Alt キーを押すと無変換キーとして動作
* 右 Alt キーを押すと変換キーとして動作
* 左右 Alt キーを押しながら他のキーを押すと Alt キーとして動作
* 左右 Alt キーを長押しすると Alt キーとして動作

長押しと認識されるまでの時間は0.15秒に設定されています。

また、個人的にミスタイプの原因となっていた次の操作について調整が加えてあります。

* 左右 Alt キーを押しながら長押しとして認識される前に A～Z キーを押すと、無変換/変換を押した後に A～Z キーを押したものとして動作


## Requirements

* Windows 10

非常に単純なプログラムなので Windows 2000 以降であれば動作するのではないかと思いますが、Windows 10 以外ではテストしていません。


## Usage

ConvertAndNonConvertEmulator.exe を起動すると常駐し、それ以後の Alt キーの操作を置き換えます。

ウィンドウやアイコンなどは一切表示されないので、動作状況の確認や終了はタスクマネージャなどから行ってください。

管理者権限を持つソフトウェアに対して機能させるには、ConvertAndNonConvertEmulator.exe を管理者権限で起動する必要があります。


## License

このソフトウェアは、[MIT License](https://github.com/t-kouyama/ConvertAndNonConvertEmulator/blob/master/LICENSE) のもとで公開されています。
