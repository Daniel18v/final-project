<?php

namespace App\Repositories;

use App\Models\Post;
use App\Interfaces\Crudable;

class PostRepository implements Crudable
{
    /**
     * Contains a instance of Chapter model
     *
     * @var \App\Models\Post
     */
    protected $post;

    /**
     * Create a new PostRepository instance.
     *
     * @param \App\Models\Post $post
     * @return void
     */
    public function __construct(Post $post)
    {
        $this->post = $post;
    }

    /**
     * Get all of the posts from the database.
     *
     * @return \Illuminate\Database\Eloquent\Collection
     */
    public static function all()
    {
        return Post::all();
    }

    /**
     * Find a post by its primary key.
     *
     * @param int $id
     * @return mixed
     */
    public static function find(int $id)
    {
        return Post::find($id);
    }

    /**
     * Update the post in the database.
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
     * Save a new post and return the instance.
     *
     * @param mixed $request
     * @return bool
     */
    public function create($request)
    {
        return $this->post->create($request);
    }

    /**
     * Get all of the active comments from the database.
     *
     * @return \Illuminate\Database\Eloquent\Collection
     */
    public static function findActive()
    {
        return Post::where('active', 1)->get();
    }

    public static function latestByNumber()
    {
        return Post::orderby('id', 'desc')->take(4)->get();
    }
}