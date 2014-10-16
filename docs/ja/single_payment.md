1件の支払い
==============
お客さまから、PayPalまたは[card.io](https://www.card.io/)を使用した決済カードによる1回払いを受け取ります。これは、(1)サーバーがその後に**認証する**必要がある**即時**支払い、または(2)サーバーがその後に**回収する**必要がある支払いの**認証**、または(3)サーバーがその後に**認証**および**回収する**必要がある**注文**の支払いのいずれかです。

_まだ実行していない場合は、プロジェクトにSDKを追加するための基本的な概要と手順の[README](README.md)を参照してください。_


概要
--------

* PayPal iOS SDKは
    1. ユーザーから支払い情報を集めるためのUIを表示します。
    2. PayPalと支払いを調整します。
    3. アプリに支払い応答を返します。
* コードは
    1. PayPal iOS SDKから支払い応答を受信します。
    2. [認証](https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/)または[回収](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#capture-the-payment)または[注文処理](https://developer.paypal.com/webapps/developer/docs/integration/direct/create-process-order/)のため、サーバーに支払い応答を送信します。
    3. ユーザーに商品またはサービスを提供します(通常はサーバー経由)。

* オプションとして、アプリで、PayPal iOS SDKがユーザーに**配送先住所**を選ばせるように指定することもできます。
* コードは
    1. PayPal iOS SDKに、アプリで提供された配送先住所および/またはユーザーのPayPalアカウントに登録されている配送先住所を表示するよう命令します。
    * PayPal iOS SDKは
        1. ユーザーが、表示された配送先住所を確認して選択できるようにします。
        2. 選択された配送先住所を、PayPalのサーバーに送信される支払い情報に追加します。
    * サーバーは
        1. 支払いの[認証](https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/)、[回収](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#capture-the-payment)、または[処理](https://developer.paypal.com/webapps/developer/docs/integration/direct/create-process-order/)を実行する際、支払い情報から配送先住所を取り出します。


サンプルコード
-----------

このドキュメントには、アプリでPayPal iOS SDKの決済APIを使用するためのサンプルコードが記載されています。

1. SDKを初期化してクライアントIDを入力します。この操作は通常、AppDelegateの`didFinishLaunchingWithOptions:`メソッドで行います。

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

2. PayPalPaymentDelegateの下位のクラス(UIViewControllerのサブクラスなど)を作成します。

    ```obj-c
    // SomeViewController.h
    #import "PayPalMobile.h"

    @interface SomeViewController : UIViewController<PayPalPaymentDelegate>
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
        // 値を変更する必要がある場合は、ここで変更できます。
        // たとえば、PayPalを受け付けてカード決済を受け付けない場合は以下を追加します。
        _payPalConfiguration.acceptCreditCards = NO;
        // ユーザーのPayPalアカウントに登録されている配送先住所から
        // 選択させたい場合は以下を追加します。
        _payPalConfiguration.payPalShippingAddressOption = PayPalShippingAddressOptionPayPal;
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

      // テスト環境で作業を開始してください。準備完了後、PayPalEnvironmentProductionに切り替えます。
      [PayPalMobile preconnectWithEnvironment:PayPalEnvironmentNoNetwork];
    }
    ```

5. 金額、通貨コード、短い説明、およびインテント(即時販売対承認/回収対注文/承認/回収)を使用して`PayPalPayment`を作成します。`invoiceNumber`や`PayPalShippingAddress` (`PayPalPayment.h`で定義)のようなオプションフィールドを含めることができます。 

    ```obj-c
    // SomeViewController.m

    - (IBAction)pay {

      // PayPal支払いの作成
      PayPalPayment *payment = [[PayPalPayment alloc] init];

      // 金額、通貨、および説明
      payment.amount = [[NSDecimalNumber alloc] initWithString:@"39.95"];
      payment.currencyCode = @"USD";
      payment.shortDescription = @"Awesome saws";

      // intentプロパティを使用して、これが「販売」の支払いであることを示します。
      // これは承認と回収の組み合わせを意味します。
      // 承認のみを実行し、回収をサーバーで処理する場合は、
      // PayPalPaymentIntentAuthorizeを使用します。
      // 注文を実行し、承認と回収をサーバーで処理する場合は、
      // PayPalPaymentIntentOrderを使用します。
      // (PayPalPaymentIntentOrderは、PayPal支払いでのみ有効です。クレジットカード決済の場合は無効です。)
      payment.intent = PayPalPaymentIntentSale;

      // アプリがお客さまから配送先住所を収集する場合、
      // または配送先住所がサーバーに格納されている場合は、ここで入力します。
      payment.shippingAddress = address; // 以前に作成されたPayPalShippingAddressオブジェクト
      
      // ここで設定できるその他のオプションフィールドはPayPalPayment.hにドキュメント化されています。
      // paymentDetails、items、invoiceNumber、custom、softDescriptorなどがあります。

      // 支払いが処理可能かどうかを確認します。
      if (!payment.processable) {
        // たとえば、金額がマイナスの場合やshortDescriptionが空の場合、
        // この支払いは処理できません。ここで処理することをおすすめします。
      }

      // 後に続きます
    ```

6. `PayPalPayment`および`PayPalConfiguration`オブジェクト(上記を参照)を使用して`PayPalPaymentViewController`を作成し、表示します。

    ```obj-c
      // 上記からの続きです

      // PayPalPaymentViewControllerを作成します。
      PayPalPaymentViewController *paymentViewController;
      paymentViewController = [[PayPalPaymentViewController alloc] initWithPayment:payment
                                                                     configuration:self.payPalConfiguration
                                                                          delegate:self];

      // PayPalPaymentViewControllerを表示します。
      [self presentViewController:paymentViewController animated:YES completion:nil];
    }
    ```

7. 完了した支払いまたは支払い取り消しを受信するデリゲートメソッドを記述します。これらのデリゲートメソッドは、モーダルビューコントローラーを破棄する必要があります。

    ```obj-c
    // SomeViewController.m

    #pragma mark - PayPalPaymentDelegateメソッド

    - (void)payPalPaymentViewController:(PayPalPaymentViewController *)paymentViewController
                     didCompletePayment:(PayPalPayment *)completedPayment {
      // 支払いは処理されました。認証とフルフィルメントのためサーバーに送信します。
      [self verifyCompletedPayment:completedPayment];

      // PayPalPaymentViewControllerを破棄します。
      [self dismissViewControllerAnimated:YES completion:nil];
    }

    - (void)payPalPaymentDidCancel:(PayPalPaymentViewController *)paymentViewController {
      // 支払いが取り消されました。PayPalPaymentViewControllerを破棄します。
      [self dismissViewControllerAnimated:YES completion:nil];
    }
    ```

8. [認証](https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/)、[回収](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#capture-the-payment)、または[処理](https://developer.paypal.com/webapps/developer/docs/integration/direct/create-process-order/)に加えて、注文から発送までの処理(フルフィルメント)などの業務に必要なその他の処理のために、支払い応答をサーバーに送信します。

   **ヒント:** **即時**支払いの場合、この時点で支払いは完了し、ユーザーには請求済みです。支払いの**承認/回収**または**注文**の場合、サーバーは実際にユーザーに請求する支払いを回収する必要があります。**サーバーにアクセスできない場合は、支払い証明を保存し、しばらくしてから再実行してください。**

    ```obj-c
    // SomeViewController.m

    - (void)verifyCompletedPayment:(PayPalPayment *)completedPayment {
      // 確認ディレクトリ全体を送信します
      NSData *confirmation = [NSJSONSerialization dataWithJSONObject:completedPayment.confirmation
                                                             options:0
                                                               error:nil];

      // サーバーに確認を送信します。サーバーは支払い証明を認証し、
      // ユーザーに商品またはサービスを提供します。サーバーにアクセスできない場合は、確認を保存し、
      // しばらくしてから再実行してください。
    }
    ```

次の手順
----------

**不正の防止** 即時支払いの場合は、必ず[支払い証明を認証](https://developer.paypal.com/webapps/developer/docs/integration/mobile/verify-mobile-payment/)してください。

支払いの承認/回収の場合、サーバーは[支払いを回収する](https://developer.paypal.com/webapps/developer/docs/integration/direct/capture-payment/#capture-the-payment)必要があります。

注文の場合、サーバーは[注文を処理する](https://developer.paypal.com/webapps/developer/docs/integration/direct/create-process-order/)必要があります。
