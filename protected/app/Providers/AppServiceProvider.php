<?php

namespace App\Providers;

use Illuminate\Support\ServiceProvider;
use Illuminate\Support\Carbon;
use App\Models\Comment;

class AppServiceProvider extends ServiceProvider
{
    /**
     * Bootstrap any application services.
     *
     * @return void
     */
    public function boot()
    {
        setlocale(LC_TIME, 'es_ES.UTF-8');
        Carbon::setUtf8(true);
        Carbon::setLocale(config('app.locale'));
//        view()->composer('blog.show', function ($view) {
//           $view->with('posts', Post::getPosts());
//        });
    }

    /**
     * Register any application services.
     *
     * @return void
     */
    public function register()
    {
        //
    }
}
