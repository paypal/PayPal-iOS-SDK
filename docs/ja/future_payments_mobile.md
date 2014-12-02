今後の支払い(Future Payments)のためのモバイルの統合
==================================

ここでは、PayPalアカウントを使用して今後の支払いを行うためにユーザーの同意を得る方法を説明します。

_まだ実行していない場合は、プロジェクトにSDKを追加するための基本的な概要と手順の[README](README.md)を参照してください。_


概要
--------

ご使用のアプリは、モバイルSDKを使って異なるタイミングで2つの情報を取得します。

最初に、PayPalアカウントからの支払いを行うため、[お客さまの同意を得る](#obtain-customer-consent)必要があります。これは以下のように実行されます。
* PayPal iOS SDKは
    1.ユーザーがPayPalアカウントの使用を承認するためのUIを表示します。
    2. 今後の支払いでPayPalを使用するための[OAuthアクセストークンスコープ](http://tools.ietf.org/html/rfc6749#page-23)に対する同意をユーザーに求めます。
    3. アプリに、OAuth2の認可コードを返します。
* アプリは
    1. SDKからOAuth2の認可コードを受け取ります。
    2. サーバーに認可コードを送ります。サーバーは[コードをOAuth2のアクセストークンおよびリフレッシュトークンと交換](future_payments_server.md#obtain-oauth2-tokens)します。

後に、事前承認された支払いの開始時に、[アプリケーション相関IDを取得する](#obtain-an-application-correlation-id)必要があります。これは以下のように実行されます。
* PayPal iOS SDKは
    * アプリケーション相関IDを提供します。
* アプリは
    * 相関IDと取引情報をサーバーに送ります。
    * サーバーは、[OAuth2のトークン、アプリケーション相関ID、および取引情報を使用して支払いを作成](future_payments_server.md)します。


お客さまの同意を得る
-----------------------

1. SDKを初期化し、クライアントIDを入力します。この操作は通常、AppDelegateの`didFinishLaunchingWithOptions:`メソッドで行います。

    ```obj-c
    - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
    {
      // ...
      [PayPalMobile initializeWithClientIdsForEnvironments:@{PayPalEnvironmentProduction : @"YOUR_CLIENT_ID_FOR_PRODUCTION",
                                                             PayPalEnvironmentSandbox : @"YOUR_CLIENT_ID_FOR_SANDBOX"}];
      // ...
      return YES;
    }
    ```
    *注: 本番環境からクライアントIDを取得していない場合は、`PayPalEnvironmentProduction`の項目を省略できます。*

2. `PayPalFuturePaymentDelegate`の下位のクラス(UIViewControllerのサブクラスとしてなど)を作成します。

    ```obj-c
    // SomeViewController.h
    #import "PayPalMobile.h"

    @interface SomeViewController : UIViewController<PayPalFuturePaymentDelegate>
    // ...
    @end
    ```

3.  `PayPalConfiguration`オブジェクトを作成します。このオブジェクトにより、SDKのさまざまな側面を設定できます。

    ```obj-c
    // SomeViewController.m

    @interface SomeViewController ()
    // ...
    @property (nonatomic, strong, readwrite) PayPalConfiguration *payPalConfiguration;
    // ...
    @end

    @implementation SomeViewController

    - (instancetype)initWithCoder:(NSCoder *)aDecoder {
      self = [super initWithCoder:aDecoder];
      if (self) {
        _payPalConfiguration = [[PayPalConfiguration alloc] init];

        // 値の詳細とデフォルト値はPayPalConfiguration.hを参照してください。

        // 最低限3つのマーチャント情報プロパティを設定する必要があります。
        // これらは、アプリを登録した際にPayPalに提供した値と同じである必要があります。
        _payPalConfiguration.merchantName = @"Ultramagnetic Omega Supreme";
        _payPalConfiguration.merchantPrivacyPolicyURL = [NSURL URLWithString:@"https://www.omega.supreme.example/privacy"];
        _payPalConfiguration.merchantUserAgreementURL = [NSURL URLWithString:@"https://www.omega.supreme.example/user_agreement"];

      }
      return self;
    }
    ```

4. 環境を確立し、PayPalのサーバーに事前接続します。

   ユーザーが支払いを開始する*可能性のある*ビューコントローラーを最初に表示する際に行うことをおすすめします。
   (接続時間は制限されているため、コントローラーの表示よりもかなり前に事前接続することはしないでください。)

    ```obj-c
    // SomeViewController.m

    - (void)viewWillAppear:(BOOL)animated {
      [super viewWillAppear:animated];

      // モック環境で作業を開始してください。準備完了後、PayPalEnvironmentProductionに切り替えます。
      [PayPalMobile preconnectWithEnvironment:PayPalEnvironmentNoNetwork];
    }
    ```

5. 設定を使用して`PayPalFuturePaymentViewController`を作成および表示します。

    ```obj-c
    // SomeViewController.m

    - (IBAction)obtainConsent {

      PayPalFuturePaymentViewController *fpViewController;
      fpViewController = [[PayPalFuturePaymentViewController alloc] initWithConfiguration:self.payPalConfiguration
                                                                                 delegate:self];

      //PayPalFuturePaymentViewControllerを表示します。
      [self presentViewController:fpViewController animated:YES completion:nil];
    }
    ```

6. `PayPalFuturePaymentDelegate`デリゲートメソッドを実装し、成功時の認可応答やユーザーによるキャンセルの通知を受信します。
 実装により、モーダルビューコントローラーを破棄する必要があります。

    ```obj-c
    // SomeViewController.m

    #pragma mark - PayPalFuturePaymentDelegateメソッド

    - (void)payPalFuturePaymentDidCancel:(PayPalFuturePaymentViewController *)futurePaymentViewController {
      // ユーザーがログインをキャンセルしました。PayPalLoginViewControllerを破棄します。
      [self dismissViewControllerAnimated:YES completion:nil];
    }

    - (void)payPalFuturePaymentViewController:(PayPalFuturePaymentViewController *)futurePaymentViewController
                    didAuthorizeFuturePayment:(NSDictionary *)futurePaymentAuthorization {
      // ユーザーは、問題なくPayPalにログインし、今後の支払いに同意しました。

      // ここでコードはサーバーに認可応答を送信する必要があります。
      [self sendAuthorizationToServer:futurePaymentAuthorization];

      // 必ずPayPalLoginViewControllerを破棄してください。
      [self dismissViewControllerAnimated:YES completion:nil];
    }
    ```

7. プロセスを完了するため、サーバーに認可応答を送信します。

    ```obj-c
    // SomeViewController.m

    - (void)sendAuthorizationToServer:(NSDictionary *)authorization {
      // 認可応答全体を送信します
      NSData *consentJSONData = [NSJSONSerialization dataWithJSONObject:authorization
                                                                options:0
                                                                  error:nil];

      // (ここにネットワークコードを記述します)
      //
      // 認可応答をサーバーに送信すると
      // 認可コードをOAuthのアクセストークンおよびリフレッシュトークンと交換できます。
      //
        // サーバーはこれらのトークンを保管するので、サーバーコードは、今後このユーザーの
        // 支払いを実行できます。
    }
    ```


アプリケーション相関IDの取得
-----------------------


モバイル端末から事前承認された支払い(「今後の支払い」)を開始する場合、モバイルアプリは、アプリケーション相関IDをモバイルSDKから取得し、サーバーに渡す必要があります。サーバーは、PayPalに送信する支払いリクエストにこのアプリケーション相関IDを含める必要があります。

PayPalは、このアプリケーション相関IDを使用して、ユーザーが同意した有効な端末およびアプリから支払いが行われていることを認証します。これは、不正取引や拒否を減らすことに役立ちます。**PayPalは、適切にアプリケーション相関IDを提供しない取引での損失についてはいっさい保護しません。**

**この値をキャッシュまたは格納しないでください。**

例:

```obj-c
- (IBAction)paymentButtonTapped:(UIButton *)sender {

    // アクティビティインジケーターを表示します。

    NSString *metadataID = [PayPalMobile clientMetadataID];

    // PayPalでの処理のため、相関IDと取引の詳細をサーバーに送信します。
}
```

サーバーは、PayPalへの支払いリクエストを作成する際、HTTPヘッダー`PayPal-Client-Metadata-Id`に、SDKから取得したこのアプリケーション相関IDの値を含める必要があります。




次の手順
----------

[今後の支払いのためのサーバー側の統合](future_payments_server.md)を読み、認可コードとOAuth2のトークンを交換して、アクセストークンと相関IDにより支払いを作成します。
