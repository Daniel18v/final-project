<?php

namespace App\Interfaces;

interface Crudable
{
    /**
     * Get all of the model from the database.
     *
     * @return \Illuminate\Database\Eloquent\Collection
     */
    public static function all();

    /**
     * Find an model by its primary key.
     *
     * @param int $id
     * @return mixed
     */
    public static function find(int $id);

    /**
     * Update the model in the database.
     *
     * @param mixed $request
     * @param int $id
     * @return bool
     */
    public function update($request, int $id);

    /**
     * Save a new model and return the instance.
     *
     * @param mixed $request
     * @return bool
     */
    public function create($request);
}
