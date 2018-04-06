<?php

namespace App\Providers;

use App\Interfaces\Crudable;
use App\Repositories\ChapterRepository;
use Illuminate\Support\ServiceProvider;

class ChapterServiceProvider extends ServiceProvider
{
    /**
     * Bootstrap services.
     *
     * @return void
     */
    public function boot()
    {
        //
    }

    /**
     * Register services.
     *
     * @return void
     */
    public function register()
    {
        app()->bind(Crudable::class, ChapterRepository::class);
    }
}
