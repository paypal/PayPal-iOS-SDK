PayPal iOS SDK
==============

PayPal iOS SDKを使用すると、モバイルアプリにPayPalおよびクレジットカード決済の機能を簡単に追加できます。

![SDKのスクリーンショット](../sdk-screens.png)

## 目次

- [ユースケース](#use-cases)
- [要件](#requirements)
- [プロジェクトにSDKを追加する](#add-the-sdk-to-your-project)
- [認証情報](#credentials)
- [海外サポート](#international-support)
- [テスト](#testing)
- [ドキュメント](#documentation)
- [ユーザビリティ](#usability)
- [PayPal iOS SDK 2.0への移行](#moving-to-paypal-ios-sdk-20)
- [次のステップ](#next-steps)


## ユースケース

SDKは、支払いについて**1件の支払い**と**今後の支払い**の2つのユースケースを、お客さまに関する情報取得について**個人設定の共有**のユースケースをサポートしています。


### 1件の支払い(Single Payment)

お客さまのPayPalアカウントまたは([card.io](https://www.card.io/)でスキャンされた)決済カードから1回のみ支払いを受け取ります。これは、(1)サーバーがその後に**認証する**必要がある**即時**支払い、または(2)サーバーがその後に**回収する**必要がある支払いの**認証**、または(3)サーバーがその後に**認証**および**回収する**必要がある**注文**の支払いのいずれかです。

1. [1件の支払いを受諾](single_payment.md)して支払い証明を受け取ります。2. サーバーで、PayPalのAPIを使用して、[支払いの認証](https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/)、[支払いの回収](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#capture-the-payment)、または[注文の処理](https://developer.paypal.com/webapps/developer/docs/integration/direct/create-process-order/) (PayPalデベロッパーサイト)を行います。


### 今後の支払い(Future Payments)

お客さまは、一度だけPayPalにログインして今後の支払いに同意します。
1. [お客さまの同意を得て](future_payments_mobile.md#obtain-customer-consent)承認コードを受け取ります。2. ご使用のサーバーで、この承認コードを使用して[OAuth2トークンを取得](future_payments_server.md#obtain-oauth2-tokens)します。

この後、お客さまが支払いを開始すると以下のプロセスに進みます。 

1. サーバーに渡す[アプリケーション相関IDを取得](future_payments_mobile.md#obtain-an-application-correlation-id)します。
2. ご使用のサーバーで、OAuth2トークン、アプリケーション相関ID、およびPayPalのAPIを使用して[支払いを作成](future_payments_server.md#create-a-payment)します。


###個人設定の共有

お客さまは、PayPalにログインして、PayPalが貴社と情報を共有することに同意します。

1. [お客さまの同意を得て](profile_sharing_mobile.md#obtain-customer-consent)承認コードを取得します。2. ご使用のサーバーで、この承認コードを使用して[OAuth2トークンを取得](profile_sharing_server.md#obtain-oauth2-tokens)します。3. ご使用のサーバーで、OAuth2トークンとPayPalのAPIを使用して、[顧客情報を検索](profile_sharing_server.md#retrieve-customer-information)します。


## 要件

* Xcode 5およびiOS SDK 7
* iOS 6.0+
* armv7、armv7s、arm64端末、およびシミュレータ(armv6以外)
* すべてのサイズおよび解像度のiPhoneおよびiPad


## プロジェクトにSDKを追加する

1. SDKをコピーまたはダウンロードします。SDKは、ヘッダーファイル、ライセンス許諾書、リリースノート、およびスタティックライブラリで構成されています。サンプルアプリも含まれています。* **バージョン2.4.0以降の場合、Xcode 6およびiOS 8 SDKが必要です。Xcode 5を使用している場合は、このSDKの[バージョン2.3.2](https://github.com/paypal/PayPal-iOS-SDK/releases)を使用してください。**
2. `PayPalMobile`ディレクトリ(複数の.hファイルおよびlibPayPalMobile.aを含む)を自分のXcodeプロジェクトに追加します。[Copy items...(項目をコピー...)]にチェックを入れて[Create groups...(グループを作成...)]を選択することをおすすめします。3. プロジェクトの**ビルドの設定** (`PROJECTS`セクションではなく`TARGETS`セクション)で以下を行います。
  * `-lc++ -ObjC`を`Other Linker Flags`に追加する
  * `Enable Modules (C and Objective-C)`を有効にする
  * `Link Frameworks Automatically`を有効にする
4. プロジェクトの**ビルドフェーズ**で、プロジェクトとそれらのライブラリをリンクします。iOSのバージョン6.0へのWeak Linkがサポートされています。* `AudioToolbox.framework`
  * `AVFoundation.framework`
  * `CoreLocation.framework`
  * `CoreMedia.framework`
  * `MessageUI.framework`
  * `MobileCoreServices.framework`
  * `SystemConfiguration.framework`
5. acknowledgments.mdから、オープンソースライセンス許諾書を[自分のアプリの許諾書](http://stackoverflow.com/questions/3966116/where-to-put-open-source-credit-information-for-an-iphone-app)に追加します。

## 認証情報

モバイル統合では、本番用とテスト用(Sandbox)にそれぞれ異なる`client_id`値が必要です。

支払いを認証または作成するためのサーバー統合では、各`client_id`に対応する`client_secret`も必要です。

PayPal API認証情報は、[PayPalデベロッパーサイトのアプリケーションページ](https://developer.paypal.com/webapps/developer/applications)を開いて、ご自分のPayPalアカウントでログインすると入手できます。

### Sandbox

このアプリケーションページにログインすると、**テスト用認証情報**が割り当てられます。これには、PayPal SandboxへのiOSの統合をテストするためのクライアントIDが含まれています。

アプリのテスト中にSDKのUIでPayPalにログインする場合は、*パーソナル*Sandboxアカウントのメールアドレスとパスワードを使用する必要があります。Sandbox用*ビジネス*アカウントの認証情報は使いません。

[Sandboxアカウント](https://developer.paypal.com/webapps/developer/applications/accounts)ページで、Sandbox用のビジネスアカウントとパーソナルアカウントを作成できます。

### 本番環境

**本番用**認証情報を入手するには、ビジネスアカウントが必要です。ビジネスアカウントをまだお持ちでない場合は、同じアプリケーションページの下部にリンクがあり、ここからアカウントを開設できます。


## 海外サポート

### ローカライズ

SDKには、多数の言語およびロケールの翻訳が組み込まれています。完全なリストは、ヘッダーファイルで参照できます。

### 通貨

SDKでは複数の通貨に対応しています。完全な最新リストは、[REST APIの国と通貨に関するドキュメント](https://developer.paypal.com/webapps/developer/docs/integration/direct/rest_api_payment_country_currency_support/)を参照してください。

決済カードとPayPal支払いでは対応通貨が異なりますのでご注意ください。`PayPalConfiguration.acceptCreditCards`プロパティを使用して決済カードの受付けを無効にしない限り、**取引を、両方の支払いタイプで対応している通貨に限定することをおすすめします。** 現在両方で対応している通貨は、USD、GBP、CAD、EUR、JPYです。

ユーザーが選択した支払いタイプで対応していない通貨を使用してアプリが取引を開始した場合、SDKはユーザーにエラーを表示し、コンソールログにメッセージを出力します。


## テスト

開発およびテスト中は、環境をSandboxまたはNoNetwork/Mockモードに設定して、実際の資金移動が行われないようにしてください。詳細は、ヘッダーファイルを参照してください。


## ドキュメント

* 使用方法の概要、統合手順をステップごとに記載した説明書、サンプルコードを含むドキュメントがSDKに用意されています。* このSDKにはサンプルアプリが含まれています。* ヘッダーファイルには詳細な説明が記載されているので、必要に応じて所定のプロパティまたはパラメータの追加の詳細情報を参照してください。
* エラーコードとサーバー側の統合手順が記載された[PayPalデベロッパーのドキュメント](https://developer.paypal.com/docs)。


## ユーザビリティ

ユーザーインターフェイスの外観と動作は、ライブラリ内で設定されます。ユーザビリティとユーザーエクスペリエンスの一貫性を保つため、提供されているヘッダーに記載された方法以外で、アプリが外観プロパティを調整したりSDKの動作を変更したりすることはおすすめしません。

特に、アプリのUI要素の外観を`UIAppearance`で変更している場合は、viewcontrollerを追加する前に変更を取り消し、viewcontrollerを破棄してから再度設定する必要があります。


## PayPal iOS SDK 2.0への移行


### 1.xからのアップグレード

メジャーバージョンの変更として、2.0で導入されるAPIは、1.x統合との下位互換性がありません。ただし、SDKは、1件の支払いについて、これまでのすべての機能を引き続きサポートしています。アップグレードは簡単です。

* SDKの初期化は、新しい`PayPalMobile`クラスのメソッドで実行されます。* `PayPalPaymentViewController`のプロパティの多くは`PayPalConfiguration`に移動しています。また`PayPalPaymentViewController`イニシャライザはそのような構成オブジェクトを取得するよう変更されています。* `PayPalPaymentDelegate`プロトコルメソッドも、パラメータとして`PayPalPaymentViewController`を含むよう変更されています。


### 旧ライブラリ

PayPalは、これまでの「Mobile Payments Libraries」(MPL)から新しいPayPal AndroidおよびiOS SDKに移行中です。
新しいモバイルSDKはPayPal REST APIに基づいています。これまでのMPLはアダプティブペイメントAPIを使用しています。

第三者、並行型、チェーン型の支払いなどの機能が使えるようになるまでは、必要に応じてMPLを使用できます。

 - [GitHubのMPL](https://github.com/paypal/sdk-packages/tree/gh-pages/MPL)
 - [MPLに関するドキュメント](https://developer.paypal.com/webapps/developer/docs/classic/mobile/gs_MPL/)

MPLに関する問題は、[sdk-packages repo](https://github.com/paypal/sdk-packages/)に提出してください。

既存のエクスプレス チェックアウトを統合しているデベロッパーまたは追加機能が必要なデベロッパーは、WebViewで[モバイルエクスプレス チェックアウト](https://developer.paypal.com/webapps/developer/docs/classic/mobile/gs_MEC/)の使用を検討することができます。


## 次のステップ

ユースケースに応じて、以下が可能です:

* [1件の支払いを受ける](single_payment.md)
* [今後の支払いを作成](future_payments_server.md)するために[ユーザーの同意を得る](future_payments_mobile.md)
* [顧客情報を検索](profile_sharing_server.md)するために[ユーザーの同意を得る](profile_sharing_mobile.md)
