個人設定共有のためのモバイルの統合
==================================

ここでは、PayPalアカウントの個人設定を共有するためにユーザーの同意を得る方法を説明します。

_まだ実行していない場合は、プロジェクトにSDKを追加するための基本的な概要と手順の[README](README.md)を参照してください。_


概要
--------

PayPalアカウントの情報を共有するため、[お客さまの同意を得る](#obtain-customer-consent)必要があります。これは以下のように実行されます。

* PayPalデベロッパーサイトで
    1. お客さまに共有を求める情報を指定します。
PayPal iOS SDKは
    1. ユーザーがPayPalアカウントの使用を承認するためのUIを表示します。
    2. PayPalを使用して個人設定を共有するための[OAuthアクセストークンスコープ](http://tools.ietf.org/html/rfc6749#page-23)に対する同意をユーザーに求めます。
    3. アプリに、OAuth2の認可コードを返します。
* アプリは
    1. SDKからOAuth2の認可コードを受け取ります。
    2. サーバーに認可コードを送ります。サーバーは[コードをOAuth2のアクセストークンおよびリフレッシュトークンと交換](profile_sharing_server.md#obtain-oauth2-tokens)します。
    3. サーバーは、[OAuth2のトークンを使用してPayPalから該当する顧客情報を要求](profile_sharing_server.md)します。

**注:**  
1. PayPal iOS SDKで使用できるスコープ値の完全なリストは`PayPalOAuthScopes.h`を参照してください。  
2. 使用できるスコープ属性の完全なリストは、[PayPalユーザー属性によるログイン](https://developer.paypal.com/docs/integration/direct/identity/attributes/)を参照してください。


共有する情報の指定
---------------------------------------

1. [PayPalデベロッパーサイト](https://developer.paypal.com)にログインします。
2. [アプリ](https://developer.paypal.com/webapps/developer/applications/myapps)を選択します。
3. `APP CAPABILITIES`の下で、`Log In with PayPal`を選択して`Advanced options`をクリックします。
4. `Information requested from customers`の下で、共有する必要がある項目(スコープ属性)を選択します。
5. `Links shown on customer consent page`の下にプライバシーポリシーおよびユーザー規約のURLを入力すると、[`PayPalConfiguration`オブジェクト](#obtain-customer-consent)に入力する対応URLが無効になります。


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

2. `PayPalProfileSharingDelegate`の下位のクラス(UIViewControllerのサブクラスなど)を作成します。

    ```obj-c
    // SomeViewController.h
    #import "PayPalMobile.h"

    @interface SomeViewController : UIViewController<PayPalProfileSharingDelegate>
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

5. `PayPalConfiguration`オブジェクトと適切なスコープ値を使用して`PayPalProfileSharingViewController`を作成し、表示します。

    ```obj-c
    // SomeViewController.m

    - (IBAction)obtainConsent {

      // 自分の場合に適用するスコープ値を選択します。使用可能なスコープ値の完全なリストは`PayPalOAuthScopes.h`を参照してください。
      NSSet *scopeValues = [NSSet setWithArray:@[kPayPalOAuth2ScopeOpenId, kPayPalOAuth2ScopeEmail, kPayPalOAuth2ScopeAddress, kPayPalOAuth2ScopePhone]];

      PayPalProfileSharingViewController *psViewController;
      psViewController = [[PayPalProfileSharingViewController alloc] initWithScopeValues:scopeValues
                                                                           configuration:self.payPalConfiguration
                                                                                delegate:self];

      // PayPalProfileSharingViewControllerを表示します。
      [self presentViewController:psViewController animated:YES completion:nil];
    }
    ```

6. `PayPalProfileSharingDelegate`デリゲートメソッドを実装し、成功時の認可応答やユーザーによるキャンセルの通知を受信します。
 実装により、モーダルビューコントローラーを破棄する必要があります。

    ```obj-c
    // SomeViewController.m

    #pragma mark - PayPalProfileSharingDelegateメソッド

    - (void)userDidCancelPayPalProfileSharingViewController:(PayPalProfileSharingViewController *)profileSharingViewController {
      // ユーザーがログインをキャンセルしました。PayPalProfileSharingViewControllerを破棄します。
      [self dismissViewControllerAnimated:YES completion:nil];
    }

    - (void)payPalProfileSharingViewController:(PayPalProfileSharingViewController *)profileSharingViewController
                 userDidLogInWithAuthorization:(NSDictionary *)profileSharingAuthorization {
      // ユーザーはPayPalにログインし、個人設定の共有に同意しました。

      // ここでコードはサーバーに認可応答を送信する必要があります。
      [self sendAuthorizationToServer:profileSharingAuthorization];

      // 必ずPayPalProfileSharingViewControllerを破棄してください。
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
      // サーバーは、これらのトークンを使用してPayPalから顧客情報を取り出す必要があります。
    }
    ```




次の手順
----------

[個人設定共有のためのサーバー側の統合](profile_sharing_server.md)を読んで、認可コードとOAuth2のトークンを交換して、PayPalから顧客情報を取り出します。
