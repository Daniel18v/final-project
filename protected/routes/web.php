<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

$route['probando'] = 'Main_controller/probando';
$route['probando/(:any)'] = 'Main_controller/probando/$1';
$route['chat'] = 'Main_controller/chat';
$route['juegos/emulador'] = 'Roms_controller/emulator';
$route['juegos/emulador/(:any)'] = 'Consoles_controller/emulator/$1';
$route['juegos/emulador/(:any)/(:any)'] = 'Roms_controller/emulator/$1/$2';
$route['juegos'] = 'Html5_controller/games';
$route['juegos/(:any)'] = 'Html5_controller/games/$1';
$route['tienda'] = 'Main_controller/shop';
$route['social'] = 'Main_controller/social';
$route['noticias'] = 'Main_controller/news';
$route['foro'] = 'Main_controller/forum';
$route['login'] = 'User_controller/login';
$route['salir'] = 'User_controller/logout';
$route['add_coins'] = 'User_controller/add_coins';
$route['buy_game'] = 'User_controller/buy_game';
$route['add_coins_comment'] = 'User_controller/add_coins_comment';
$route['registro'] = 'User_controller/signup';
$route['subida'] = 'Upload_controller/upload';
$route['admin'] = 'ACP_controller/admin';
$route['blog'] = 'Main_controller/blog';
$route['blog/(:any)'] = 'Blog_controller/blog/$1';
$route['blog/(:any)/comment'] = 'Blog_controller/blog_comment/$1';
$route['admin/blog'] = 'ACP_controller/admin/blog';
$route['admin/blog/(:any)'] = 'Blog_controller/blog_actions/$1';
$route['perfil'] = 'UCP_controller/profile';
$route['perfil/editar'] = 'UCP_controller/edit';
$route['perfil/actualizar'] = 'UCP_controller/update_profile';
$route['perfil/mis-juegos'] = 'UCP_controller/show_games';
$route['perfil/(:any)'] = 'UCP_controller/profile/$1';








//Home
Route::get('/', 'HomeController@index')->name('/');

//Contact
Route::view('contacto', 'contact.index')->name('contact')->middleware('admin');

//Blog and comments
Route::group(['prefix' => 'blog', 'as' => 'blog.'] ,function () {
    Route::get('/', 'PostController@index')->name('index');
    Route::get('{post}', 'PostController@show')->name('show');
    //Posting
    Route::post('{post}/comments', 'CommentController@store')->name('comments.store');
});

//Animes
Route::group(["prefix" => 'animes', 'as' => 'animes.'], function () {
    Route::get('crear', 'AnimeController@create')->name('create');
    Route::get('/', 'AnimeController@index')->name('index');
    Route::post('/', 'AnimeController@store')->name('store');
    Route::get('{anime}/editar', 'AnimeController@edit')->name('edit');
    Route::get('{anime}', 'AnimeController@show')->name('show');
    Route::patch('{anime}', 'AnimeController@update')->name('update');
    Route::delete('{anime}', 'AnimeController@destroy')->name('destroy');
    //Chapters
    Route::group([ 'prefix' => '{anime}', 'as' => 'chapter.'], function () {
        Route::get('crear', 'ChapterController@create')->name('create');
        Route::get('{chapter}', 'ChapterController@show')->name('show');
        Route::post('/', 'ChapterController@store')->name('store');
        Route::get('{chapter}/editar', 'ChapterController@edit')->name('edit');
        Route::patch('{chapter}', 'ChapterController@update')->name('update');
        Route::delete('{chapter}', 'ChapterController@destroy')->name('destroy');
    });
});

//Authentification
Auth::routes();
Route::get('salir', 'Auth\LoginController@logout')->name('logout');