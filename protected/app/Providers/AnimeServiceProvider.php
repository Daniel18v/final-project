<?php

namespace App\Providers;

use App\Interfaces\Crudable;
use App\Repositories\AnimeRepository;
use Illuminate\Support\ServiceProvider;

class AnimeServiceProvider extends ServiceProvider
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
        app()->bind(Crudable::class, AnimeRepository::class);
    }
}
