_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
【 ソフト名 】　TimeStamp Keeper
【バージョン】　1.02
【 製作者名 】　x@rgs
【 動作環境 】　Windows XP以降
【 製作言語 】　C++
【ｿﾌﾄｳｪｱ種別】　フリーソフトウェア
【 配布条件 】　NYSL Version 0.9982
【  連絡先  】	Y.R.Takanashi@gmail.com
【  サイト  】	http://www16.atpages.jp/rayna/index.html
_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
●はじめに
 この度は、「TimeStamp Keeper」(以下、本ソフトとも)をダウンロードしていただき、ありがとうございます。
 以下、稚拙な文章が続きますが、もしかしたら重要な情報が含まれるかもしれませんので、目を通していただけると幸いです。



●説明
 「TimeStamp Keeper」はファイルのタイムスタンプを記憶し、ワンクリックで元に戻すソフトウェアです。
 タイムスタンプを変更したくない、ちょっとした編集をしたいとき、
 勝手に書き換えられるタイムスタンプを保持したいとき、
 等々の場面で活躍します。



●特徴
 ・ファイル登録はドラッグ&ドロップ、復元はEnterキーひとつの簡単動作。



●ファイル構成
 NYSL_withfaq.TXT        ----- ライセンスファイル
 Readme.txt              ----- このファイル
 Src.7z                  ----- TimeStamp Keeperソースファイル
 TimeStampKeeper.exe     ----- TimeStamp Keeper本体ファイル



●インストール
 1.「TimeStampKeeper102.zip」を適当なディレクトリに解凍して下さい。
 2.「送る」メニュー(SendToフォルダ)や、デスクトップに「TimeStampKeeper.exe」へのショートカットを作成しておくと便利です。



●アンインストール
 レジストリは一切使用しないため、ディレクトリごと、若しくはコピーしたファイルを削除して下さい。



●使用方法
 1.タイムスタンプを保持したいファイルをドラッグ&ドロップで追加してください。
  ・TimeStamp Keeperウインドウでも、本体ファイル「TimeStampKeeper.exe」でも可能です。
  ・Ctrlキーを押下しながら「TimeStampKeeper.exe」にドロップすると、ウインドウを前面に表示させません。
  ・チェックボックスはSpaceキーで反転します。

 2.タイムスタンプを元に戻したい時に、Enterキーを押下するか、「復元」をクリックしてください。
  ・成功 or 失敗の背景色は、チェックを外すと元に戻ります。

 3.F2キーを押下するか、「編集」をクリックすると、ファイル名や更新日時を編集することができます。

 4.「設定」->「再帰的に追加」を有効にすると、ディレクトリ以下すべてのディレクトリ・ファイルを追加します。
  ・引数「/r」を付加して起動すると、あらかじめ有効化することができます。



●注意
 ・最終更新日時のみ復元します。
 ・読み取り専用属性のファイルは、属性を一時的に変更して、タイムスタンプを復元します。



●開発環境
 [Main]
 OS:Microsoft Windows 7 Home Premium 64-bit (6.1, Build 7600)
 CPU:Intel(R) Core(TM) i5 CPU M 460 @ 2.53GHz (4 CPUs), ~2.5GHz
 memory:4096MB RAM
 compilerdebugger:Microsoft Visual C++ 2015(Microsoft Visual Studio Community 2015)
                  Microsoft Visual C++ 2010 Express
 editor:xyzzy version 0.2.2.235/ResEdit 1.6.4 Unicode build.

 [Sub]
 OS:Microsoft Windows XP Home Edition Build 2600 SP3
 CPU:Intel(R) Atom(TM) CPU N270@1.60GHz,1600MHz(4x400)
 memory:1016MB
 compiler/debugger:Microsoft Visual C++ 2010 Express
 editor:xyzzy version 0.2.2.235/ResEdit 1.6.4 Unicode build.


●謝辞
 本ソフトの開発にあたって、様々なソフトウェアたちを利用させていただきました。
 この場を借りて御礼申し上げます。


●ライセンス
 ・本ソフトはNYSL Version 0.9982に準拠して配布されています。
   ライセンスの詳細は同梱の「NYSL_withfaq.TXT」をご覧下さい。


●その他
 サポート(ご意見、ご感想、不具合報告、要望等)は
 Y.R.Takanashi@gmail.com
 若しくは
 BBS(以下URLよりアクセス可能)にて、

 最新版は
 http://www16.atpages.jp/rayna/index.html
 でお願いします。


●開発履歴
 ○Ver.1.02 - 2015/12/12
 ・コンパイラをMicrosoft Visual C++ 2015に変更、静的ビルドに変更。
   「Microsoft Visual C++ 20xx 再頒布可能パッケージ」のインストールが不要になりました。
 ・「再帰的に追加」オプションを追加。引数「/r」であらかじめ有効化することができます。

 ○Ver.1.01 - 2015/10/07
 ・起動済みプロセスがなくとも、Ctrlキー押下で最小化起動するように。
 ・ファイル名、更新日時の編集ができるように。
 ・エラーダイアログを表示しないように。

 ○Ver.1.00 - 2015/03/20
 ・初公開。



_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
This Readme file made by x@rgs
