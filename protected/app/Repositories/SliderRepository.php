<?php

namespace App\Repositories;

use App\Models\Slider;
use App\Interfaces\Crudable;

class SliderRepository implements Crudable
{
    /**
     * Contains a instance of Chapter model
     *
     * @var \App\Models\Slider
     */
    protected $slider;

    /**
     * Create a new SliderRepository instance.
     *
     * @param \App\Models\Slider $slider
     * @return void
     */
    public function __construct(Slider $slider)
    {
        $this->slider = $slider;
    }

    /**
     * Get all of the sliders from the database.
     *
     * @return \Illuminate\Database\Eloquent\Collection
     */
    public static function all()
    {
        return Slider::all();
    }

    /**
     * Find a slider by its primary key.
     *
     * @param int $id
     * @return mixed
     */
    public static function find(int $id)
    {
        return Slider::find($id);
    }

    /**
     * Update the slider in the database.
     *
     * @param mixed $request
     * @param int $id
     * @return bool
     */
    public function update($request, int $id)
    {
        //TODO
        return false;
    }

    /**
     * Save a new slider and return the instance.
     *
     * @param mixed $request
     * @return bool
     */
    public function create($request)
    {
        //TODO
        return false;
    }

    /**
     * Get all of the active comments from the database.
     *
     * @return \Illuminate\Database\Eloquent\Collection
     */
    public static function findActive()
    {
        return Slider::where('active', 1)->get();
    }
}