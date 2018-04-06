<?php

namespace App\Repositories;

use App\Models\Comment;
use App\Interfaces\Crudable;

class CommentRepository implements Crudable
{
    /**
     * Contains a instance of Comment model
     *
     * @var \App\Models\Comment
     */
    protected $comment;

    /**
     * Create a new CommentRepository instance.
     *
     * @param \App\Models\Comment $comment
     * @return void
     */
    public function __construct(Comment $comment)
    {
        $this->comment = $comment;
    }

    /**
     * Get all of the comments from the database.
     *
     * @return \Illuminate\Database\Eloquent\Collection
     */
    public static function all()
    {
        return Comment::all();
    }

    /**
     * Find a comment by its primary key.
     *
     * @param int $id
     * @return mixed
     */
    public static function find(int $id)
    {
        return Comment::find($id);
    }

    /**
     * Get all of the active comments from the database.
     *
     * @return \Illuminate\Database\Eloquent\Collection
     */
    public static function findActive()
    {
        return Comment::where('active', 1)->get();
    }

    /**
     * Update the comment in the database.
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
     * Save a new comment and return the instance.
     *
     * @param mixed $request
     * @return bool
     */
    public function create($request)
    {
        if ($this->comment->create([
            'body' => request('body'),
            'post_id' => $request->id,
            'user_id' => auth()->user()->id
        ])) {
            return redirect(url("blog/{$request->slug}"))
                ->with('postOk', __('The comment has been succesfully created'));
        }
    }
}