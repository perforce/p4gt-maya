/*
 * Copyright 2004 Perforce Software.  All rights reserved.
 *
 * Developed by Data Shades Ltd.
 *
 * This file contains all plugin text and messages
 *
 * NOTE: This file MUST be saved in UTF-8 with BOM for the plug-ins to display the Japanese text.
 */

/*
 * Messages used in ..\photoshop\PerforceMenu.cpp, ..\softimage\P4GTSoftImage.cpp
 */
											// "&Add to P4"
#define	P4_MENU_ADD							_T("P4に追加(&A)")

											// "Check &Out"
#define	P4_MENU_CHECKOUT					_T("チェックアウト(&O)")

											// "Check &In..."
#define	P4_MENU_CHECKIN						_T("チェックイン(&I)...")

											// "&Result of Last Command..."
#define	P4_MENU_RESULT_CMD					_T("最後のコマンドの結果(&R)...")

											// "File &Status..."
#define	P4_MENU_DOC_STAT					_T("ファイルの状態(&S)...")

											// "Do&cument History..."
#define	P4_MENU_DOC_HIST					_T("ファイルの履歴(&C)...")

											// "Checked Out &Files..."
#define	P4_MENU_DOC_CHECKED_OUT				_T("チェックアウトされたファイル(&D)...")

											// "&P4 Information..."
#define	P4_MENU_SERVER_INFO					_T("P4の情報(&P)...")

											// "Get Latest Revision &from P4..."
#define	P4_MENU_GET_LATEST					_T("P4から最新リビジョンを取得(&F)...")

											// "Op&tions..."
#define	P4_MENU_OPTIONS						_T("オプション(&T)...")

											// "&Help..."
#define	P4_MENU_HELP						_T("ヘルプ(&H)...")

											// "A&bout P4GT..."
#define	P4_MENU_ABOUT						_T("P4GTについて(&B)...")

											// "Ser&ver Online"
#define	P4_MENU_SERVER_ONLINE				_T("接続中のサーバ(&V)")

											// "&P4"
#define	P4_MENU_PERFORCE					_T("P4(&P)")

											// "Lock File"
#define	P4_MENU_LOCK						_T("ファイルをロック")

											// "Unlock File"
#define	P4_MENU_UNLOCK						_T("ファイルのロックを解除")

											// "Lock/Unlock File"
#define	P4_MENU_LOCK_UNLOCK					_T("ファイルをロック/ロック解除")

											// "Undo Add"
#define	P4_MENU_UNDO_ADD					_T("追加を元に戻す")

											// "Undo Check Out"
#define	P4_MENU_UNDO_CHECKOUT				_T("チェックアウトを元に戻す")

											// "Undo Add/Check Out"
#define	P4_MENU_UNDO_ADD_CHECKOUT			_T("追加/チェックアウトを元に戻す")

/*
 * Messages used in PerforceConnection.cpp
 */
											// "P4 password (P4PASSWD) invalid or unset."
#define	P4_CONNECTION_INVALID_PASSWORD		_T("P4パスワード(P4PASSWD)が不正または未設定です")

											// "please login again"
#define	P4_CONNECTION_LOGIN					_T("再度ログインしてください")

/*
 * Messages used in ExternalRefDialog.cpp
 */
											// "Name"
#define	P4_EXTERNALREF_NAME					_T("名前");

											// "Have Revision"
#define	P4_EXTERNALREF_HAVE					_T("所有リビジョン");

											// "Head Revision"
#define	P4_EXTERNALREF_HEAD					_T("最新リビジョン");

/*
 * Messages used in FileOpDialog.cpp
 */
											// "Depot file: "
#define	P4_FILEOP_DEPOT_FILE				_T("ディポ・ファイル: ")

											// "Your File: "
#define	P4_FILEOP_YOUR_DOC					_T("ユーザのファイル: ")

											// "Latest (head) action in depot: "
#define	P4_FILEOP_ACTION					_T("ディポ内での最新アクション: ")

											// "File type: "
#define	P4_FILEOP_TYPE						_T("ファイルタイプ: ")

											// "Last modified in depot: "
#define	P4_FILEOP_LAST_MOD					_T("ディポ内での最新の更新: ")

											// "Latest (head) revision in depot: "
#define	P4_FILEOP_HEAD_REV					_T("ディポ内の最新リビジョン: ")

											// "Latest (head) changelist in depot: "
#define	P4_FILEOP_HEAD_CHG					_T("ディポ内の最新チェンジリスト: ")

											// "Revision in your workspace: "
#define	P4_FILEOP_HAVE						_T("ワークスペース内のリビジョン: ")

											// "Open by you for: "
#define	P4_FILEOP_OPENED_YOU_FOR			_T("現ユーザによりこの目的でオープン: ")

											// "In changelist: "
#define	P4_FILEOP_IN_CHG					_T("チェンジリスト: ")

											// "Open by P4 user: "
#define	P4_FILEOP_OPENED_OTHER				_T("P4ユーザによりオープン: ")

											// "Also open by: "
#define	P4_FILEOP_OPENED_OTHER_ALSO			_T("他にオープンしているユーザ: ")

											// "Opened by them for: "
#define	P4_FILEOP_OPENED_OTHER_FOR			_T("他ユーザによりこの目的でオープン: ")

											// "Locked by you"
#define	P4_FILEOP_LOCKED_YOU				_T("現ユーザによりロック")

											// "Locked by someone else"
#define	P4_FILEOP_LOCKED_OTHER				_T("他ユーザによりロック")

											// "P4GT - Confirmation of Check Out from P4"
#define	P4_FILEOP_CONF_CHECKOUT				_T("P4GT - P4からのチェックアウトの確認　")

											// "P4GT - Confirmation of Add to P4"
#define	P4_FILEOP_CONF_ADD					_T("P4GT -P4への追加の確認")

											// "P4GT - Confirmation of Undo Add/Check Out"
#define	P4_FILEOP_CONF_ADD_CHECKOUT			_T("P4GT - 元に戻す/チェックアウトの確認")

											// "P4GT - Confirmation of Lock File"
#define	P4_FILEOP_CONF_LOCK					_T("P4GT - ファイルのロックの確認")

											// "P4GT - Confirmation of Unlock File"
#define	P4_FILEOP_CONF_UNLOCK				_T("P4GT - ファイルのロック解除の確認")

											// "P4GT - Confirmation of Check In to P4"
#define	P4_FILEOP_CONF_CHECKIN				_T("P4GT - P4へのチェックインの確認")

											// "P4GT - File Status"
#define	P4_FILEOP_DOC_STATUS				_T("P4GT - ファイルの状態")

/*
 * Messages used in FullSubmitDialog.cpp
 */
											// "Files:"
#define	P4_FULLSUBMIT_DOC					_T("ファイル:")

											// "\nJobs:\n"
#define	P4_FULLSUBMIT_JOBS					_T("\nジョブ:\n")

											// DO NOT TRANSLATE NEXT LINE
#define	P4_FULLSUBMIT_DESC					_T("<コメントをここに入力>")

											// "Check In to P4"
#define	P4_FULLSUBMIT_CHECKIN				_T("P4へチェックイン")

											// "Please select at least one file."
#define	P4_FULLSUBMIT_SELECT				_T("ファイルを1つ以上選択してください")

											// "(default)"
#define	P4_FULLSUBMIT_DFLT					_T("(デフォルト)")

											// "Submit all selected files"
#define P4_FULLSUBMIT_OPTION1				_T("選択された全てのファイルをサブミット")

											// "Don't submit unchanged files"
#define P4_FULLSUBMIT_OPTION2				_T("未変更のファイルをサブミットしない")

											// "Revert unchanged files"
#define P4_FULLSUBMIT_OPTION3				_T("未変更のファイルを元に戻す")

/*
 * Messages used in HistoryDialog.cpp
 */
											// "Ver-"
#define	P4_HISTORY_VER						_T("Ver-")

											// "Revision"
#define	P4_HISTORY_REV						_T("リビジョン")

											// "Changelist"
#define	P4_HISTORY_CHG						_T("チェンジリスト")

											// "Date"
#define	P4_HISTORY_DATE						_T("日付")

											// "User"
#define	P4_HISTORY_USER						_T("ユーザ")

											// "Action"
#define	P4_HISTORY_ACTION					_T("アクション")

											// "Description"
#define	P4_HISTORY_DESC						_T("コメント")

											// "File History for "
#define	P4_HISTORY_HIST						_T("ファイルの履歴")

											// "Description for changelist "
#define	P4_HISTORY_DESC_CHG					_T("チェンジリストに関するコメント ")

/*

 * Messages used in MenuState.cpp
 */
											// "Lock/Unlock File"
#define	P4_MENUSTATE_LOCK_UNLOCK			_T("ロック/ロック解除")

											// "Undo Add/Check Out"
#define	P4_MENUSTATE_UNDO_ADD_CHECKOUT		_T("追加/チェックアウトを元に戻す")

											// "Disconnect from Ser&ver"
#define	P4_MENUSTATE_DISC					_T("サーバとの接続を切断(&V)")

											// "Connect to Ser&ver"
#define	P4_MENUSTATE_CONN					_T("サーバに接続(&V)")

											// "Un&lock File"
#define	P4_MENUSTATE_UNLOCK					_T("ファイルのロックを解除(&L)")

											// "&Lock File"
#define	P4_MENUSTATE_LOCK					_T("ファイルをロック(&L)")

											// "U&ndo Add"
#define	P4_MENUSTATE_UNDO_ADD				_T("追加を元に戻す(&N)")

											// "U&ndo Check Out"
#define	P4_MENUSTATE_UNDO_CHECKOUT			_T("チェックアウトを元に戻す(&N)")

/*
 * Messages used in OldVersionDialog.cpp
 */
											// "The File '"
#define	P4_OLDVERSION_DOC					_T("ファイル '")

											// "' is in P4."
#define	P4_OLDVERSION_CHECKED_IN			_T("' はP4の中にあります")

											// "You have an old version of this File ("
#define	P4_OLDVERSION_OLD_REV				_T("このファイルの古いバージョンを持っています(")

											// ") and the newest version in the P4 depot is "
#define	P4_OLDVERSION_HEAD_REV				_T(") P4ディポ内の最新バージョンは ")

/*
 * Messages used in OpenDepotDialog.cpp
 */
											// "All Files (*.*)"
#define	P4_OPENDEPOT_ALL					_T("すべてのファイル(*.*)")

											// "Name"
#define	P4_OPENDEPOT_NAME					_T("名前")

											// "Have Revision"
#define	P4_OPENDEPOT_HAVE					_T("所有リビジョン")

											// "Head Revision"
#define	P4_OPENDEPOT_HEAD					_T("最新リビジョン")

											// "Type"
#define	P4_OPENDEPOT_TYPE					_T("タイプ")

											// "Locked by"
#define	P4_OPENDEPOT_LOCKED_BY				_T("ロック実行者")

											// "Large Icons"
#define	P4_OPENDEPOT_LARGE_ICON				_T("大きいアイコン")

											// "Small Icons"
#define	P4_OPENDEPOT_SMALL_ICON				_T("小さいアイコン")

											// "List"
#define	P4_OPENDEPOT_LIST					_T("リスト")

											// "Details"
#define	P4_OPENDEPOT_DETAILS				_T("詳細")

											// "Up One Level"
#define	P4_OPENDEPOT_UP_ONE					_T("1つ上のレベルへ")

											// "you"
#define	P4_OPENDEPOT_YOU					_T("現ユーザ")

/*
 * Messages used in OpenFilesDialog.cpp
 */
#define	P4_OPENFILES																_T("Files:")

/*
 * Messages used in PasswordDialog.cpp
 */
											// "A password is required for user '"
#define	P4_PASSWORD_PASSWORD				_T("このユーザにはパスワードが必要です '")

											// "' on server '"
#define	P4_PASSWORD_SERVER					_T("' サーバ '")

/*
 * Messages used in PluginOperations.cpp
 */
											// "P4GT - Add File to P4"
#define	P4_OPERATIONS_ADD_DOC				_T("P4GT - ファイルをP4に追加")

											// "You have successfully added the File to P4 -\r\n"
											// "would you like to check it in now?"
#define	P4_OPERATIONS_ADDED_DOC				_T("ファイルのP4への追加が成功しました -\r\n") \
											_T("すぐにチェックインしますか?")

											// "P4GT - Opening File"
#define	P4_OPERATIONS_OPENING_DOC			_T("P4GT - ファイルを開く")

											// "This File is in P4.\r\n\r\n"
											// "Click OK to check out."
#define	P4_OPERATIONS_DOC_IN_P4				_T("このファイルはP4の中にあります\r\n\r\n") \
											_T("チェックアウトするにはOKをクリックします")

											// "P4GT - Closing File"
#define	P4_OPERATIONS_CLOSING_DOC			_T("P4GT - ファイルを閉じる")

											// "This File is checked-out from P4.\r\n\r\n"
											// "Do want to check-in this file?"
#define	P4_OPERATIONS_DOC_CHECKED_OUT		_T("このファイルはP4からチェックアウトされています\r\n\r\n") \
											_T("このファイルをチェックインしますか?")

											// "P4GT - P4 Error"
#define	P4_OPERATIONS_SERVER_ERROR			_T("P4GT - P4エラー")

											// "P4GT - Open From P4"
#define	P4_OPERATIONS_OPEN					_T("P4GT - P4から開く")

											// "Save the latest changes to the current file before opening P4 file?"
#define	P4_OPERATIONS_SAVE_LATEST			_T("P4ファイルを開く前に、現在のファイルへの最新の変更を保存しますか?")

											// "P4GT - Error Adding File to P4"
#define	P4_OPERATIONS_ADD_ERROR				_T("P4GT - ファイルのP4への追加においてエラーが発生しました")

											// "You need to save the File in a directory in the client workspace (P4 controlled area) before you can add it to P4."
#define	P4_OPERATIONS_SAVE_DOC_ADD			_T("クライアント・ワークスペース内のディレクトリ（P4の制御領域）にファイルを保存してから、P4に追加する必要があります。")

											// "P4GT - View Previous Version"
//#define	P4_OPERATIONS_VIEW_PREV				_T("P4GT - 前のバージョンを表示")
#define	P4_OPERATIONS_VIEW_PREV				_T("P4GT - 前のバージョンを見る")

											// "Save the latest changes to the current file before loading previous version?"
#define	P4_OPERATIONS_SAVE_LATEST_PREV		_T("前のバージョンをロードする前に、最新の変更を現在のファイルに保存しますか?")

											// "Are you sure you wish to discard changes and revert to the previous version?"
#define	P4_OPERATIONS_REVERT_PREV			_T("本当に変更を破棄して前のバージョンに戻しますか?")

											// "P4GT - Undo Check Out"
#define	P4_OPERATIONS_UNDO_CHECKOUT			_T("P4GT - チェックアウトを元に戻す")

											// "P4GT - Check In to P4"
#define	P4_OPERATIONS_CHECKIN				_T("P4GT - P4にチェックインする")

											// "Save the latest changes before checking in to P4?"
#define	P4_OPERATIONS_SAVE_LATEST_CHECKIN	_T("P4にチェックインする前に、最新の変更を保存しますか?")

											// "Unable to do Check In as file is not checked out."
#define	P4_OPERATIONS_NOT_CKECKED_OUT		_T("ファイルがチェックアウトされていないため、チェックインを実行できません")

											// "P4GT - Error"
#define	P4_OPERATIONS_ERROR					_T("P4GT - エラー")

											// "Possible installer problem. Unable to find value named HelpFileURL under this registry key: "
#define	P4_OPERATIONS_HELP_REGISTRY_NOT_FOUND        _T("Possible installer problem. Unable to find value named HelpFileURL under this registry key: ")

											// "Unable to show P4GT help file: "
#define	P4_OPERATIONS_SHOW_HELP_ERROR		_T("Unable to show P4GT help file: ")

											// "This File is checked-out from P4.\r\n\r\n"
											// "Do you want to revert this file?"
#define P4_OPERATIONS_CLOSE_REVERT_MSG		_T("このファイルはP4からチェックアウトされています\r\n\r\n") \
											_T("このファイルを元に戻しますか?")

											// "The File '"
#define	P4_OPERATIONS_DOC					_T("ファイル '")

											// "' is in P4.\r\n\r\n"
#define	P4_OPERATIONS_IN_PERFORCE			_T("' はP4の中にあります\r\n\r\n")

											// "Click OK to check out."
#define	P4_OPERATIONS_OK_CHECKOUT			_T("チェックアウトするにはOKをクリックします")

											// "check $P4PORT"
#define	P4_OPERATIONS_CHECK_P4PORT			_T("$P4PORTをチェックしてください")

											// "This File is checked-in to P4.\r\n\r\n"
#define	P4_OPERATIONS_CHECKED_IN			_T("このファイルはP4にチェックインされています\r\n\r\n")

											// "The File is currently exclusively locked by someone else: "
#define	P4_OPERATIONS_EXCL_LOCKED			_T("このファイルは現在、他のユーザによって排他ロックされています: ")

											// "Check Out is not possible at this time.\r\n"
#define	P4_OPERATIONS_UNABLE_CHECKOUT		_T("現時点ではチェックアウトを実行できません\r\n")

											// "P4GT - Check Out File"
#define	P4_OPERATIONS_CHECKOUT				_T("P4GT - ファイルをチェックアウト")

											// "This File is checked-in to P4.\r\n\r\n"
#define	P4_OPERATIONS_CHECKED_IN			_T("このファイルはP4にチェックインされています\r\n\r\n")

											// "The File has already been locked by someone else: "
#define	P4_OPERATIONS_LOCKED_BY				_T("このファイルはすでに他のユーザによってロックされています: ")

											// "Automatic locking after Check Out is not possible.\r\n"
#define	P4_OPERATIONS_UNABLE_AUTO_LOCK		_T("チェックアウト後に自動的にロックすることはできません\r\n")

											// "Do you still want to Check Out without locking it?"
#define	P4_OPERATIONS_CHECKOUT_NO_LOCK		_T("ロックを実行せずにチェックアウトしますか?")

											// "P4GT - Check Out File"
#define	P4_OPERATIONS_CHECKOUT				_T("P4GT - ファイルをチェックアウト")

											// "This File has been Checked Out. Do you now want to lock the File?"
#define	P4_OPERATIONS_LOCK					_T("このファイルはチェックアウトされています。今ファイルのロックを実行しますか?")

											// "is not under client's root"
#define	P4_OPERATIONS_NOT_UNDER_ROOT		_T("はクライアント・ルートの配下にありません")

											// "file(s) not in client view"
#define	P4_OPERATIONS_NOT_IN_VIEW			_T("ファイルがクライアント・ビューにありません")

											// "This file cannot be added to P4 as it is outside the client root "
#define	P4_OPERATIONS_UNABLE_ADD_ROOT		_T("このファイルは、クライアント・ルートの外にあるためP4に追加できません")

											// "This file cannot be added to P4 as it is outside the client view."
#define	P4_OPERATIONS_UNABLE_ADD_VIEW		_T("このファイルは、クライアント・ビューの外にあるためP4に追加できません")

/*
 * Messages used in ResultsDialog.cpp
 */
											// "User name: "
#define	P4_RESULTS_USER						_T("ユーザ名: ")

											// "Client name: "
#define	P4_RESULTS_CLIENT					_T("クライアント名: ")

											// "Client host: "
#define	P4_RESULTS_CLIENT_HOST				_T("クライアントのホスト: ")

											// "Client root: "
#define	P4_RESULTS_CLIENT_ROOT				_T("クライアント・ルート: ")

											// "Current directory: "
#define	P4_RESULTS_CLIENT_CWD				_T("現在のディレクトリ: ")

											// "Client address: "
#define	P4_RESULTS_CLIENT_ADDR				_T("クライアントのアドレス: ")

											// "Unicode: "
#define	P4_RESULTS_UNICODE					_T("Unicode: ")

											// "Server address: "
#define	P4_RESULTS_SERVER_ADDR				_T("サーバのアドレス: ")

											// "Server root: "
#define	P4_RESULTS_SERVER_ROOT				_T("サーバ・ルート: ")

											// "Server date: "
#define	P4_RESULTS_SERVER_DATE				_T("サーバ日付: ")

											// "Server version: "
#define	P4_RESULTS_SERVER_VERS				_T("サーバのバージョン: ")

											// "Server license: "
#define	P4_RESULTS_SERVER_LIC				_T("サーバ・ライセンス: ")

											// "P4GT - Confirmation of Check In"
#define	P4_RESULTS_CONF_CHECKIN				_T("P4GT - チェックインの確認")

											// "P4GT - Result of Last P4 Command"
#define	P4_RESULTS_RESULT_CMD				_T("P4GT - 最後のP4コマンドの結果")

											// "P4GT - P4 Connection Information"
#define	P4_RESULTS_CONN_INFO				_T("P4GT - P4接続情報")

/*
 * Messages used in TabbedOptionsDialog.cpp
 */
											// "Check In/Check Out"
#define	P4_TABBEDOPTIONS_CHECKIN_CHECKOUT	_T("チェックイン/チェックアウト")

											// "Open/Close"
#define	P4_TABBEDOPTIONS_OPEN_CLOSE			_T("開く/閉じる")
