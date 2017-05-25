<?php
defined('BASEPATH') OR exit('No direct script access allowed');

/*
| -------------------------------------------------------------------------
| URI ROUTING
| -------------------------------------------------------------------------
| This file lets you re-map URI requests to specific controller functions.
|
| Typically there is a one-to-one relationship between a URL string
| and its corresponding controller class/method. The segments in a
| URL normally follow this pattern:
|
|	example.com/class/method/id/
|
| In some instances, however, you may want to remap this relationship
| so that a different class/function is called than the one
| corresponding to the URL.
|
| Please see the user guide for complete details:
|
|	https://codeigniter.com/user_guide/general/routing.html
|
| -------------------------------------------------------------------------
| RESERVED ROUTES
| -------------------------------------------------------------------------
|
| There are three reserved routes:
|
|	$route['default_controller'] = 'welcome';
|
| This route indicates which controller class should be loaded if the
| URI contains no data. In the above example, the "welcome" class
| would be loaded.
|
|	$route['404_override'] = 'errors/page_missing';
|
| This route will tell the Router which controller/method to use if those
| provided in the URL cannot be matched to a valid route.
|
|	$route['translate_uri_dashes'] = FALSE;
|
| This is not exactly a route, but allows you to automatically route
| controller and method names that contain dashes. '-' isn't a valid
| class or method name character, so it requires translation.
| When you set this option to TRUE, it will replace ALL dashes in the
| controller and method URI segments.
|
| Examples:	my-controller/index	-> my_controller/index
|		my-controller/my-method	-> my_controller/my_method
*/
$route['default_controller'] = 'Main_controller';
$route['404_override'] = 'errors/page_missing';
$route['translate_uri_dashes'] = FALSE;
$route['probando'] = 'Main_controller/probando';
$route['probando/(:any)'] = 'Main_controller/probando/$1';
$route['chat'] = 'Main_controller/chat';
$route['emulador'] = 'Emulators_controller/emulator';
$route['emulador/(:any)'] = 'Emulators_controller/emulator/$1';
$route['emulador/(:any)/(:any)'] = 'Emulators_controller/emulator/$1/$2';
$route['juegos'] = 'Games_controller/games';
$route['juegos/(:any)'] = 'Games_controller/games/$1';
$route['tienda'] = 'Main_controller/shop';
$route['social'] = 'Main_controller/social';
$route['noticias'] = 'Main_controller/news';
$route['blog'] = 'Main_controller/blog';
$route['foro'] = 'Main_controller/forum';
$route['login'] = 'User_controller/login';
$route['salir'] = 'User_controller/logout';
$route['registro'] = 'User_controller/signup';
$route['subida'] = 'Upload_controller/upload';
$route['admin'] = 'ACP_controller/admin';