 // CefLibrary := 'library\libcef.dll';
  CefCache := 'cache';
  CefUserAgent := 'ArcadeBrowser/1.0 (Windows) Chromium/31.0.1650.57 CEF 3.1650.1562';
  CefLocale := 'en';
  CefLocalesDirPath := 'locales';
  CefResourcesDirPath := 'resources';
 // CefRegisterCustomScheme('dcef', True, False, False);
  CefRegisterSchemeHandlerFactory('dcef', '', True, TFileScheme);
  CefRegisterSchemeHandlerFactory('local', '', False, TFileScheme);
