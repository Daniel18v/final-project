<?php

namespace App\Http\Controllers;

use App\Models\Post;
use App\Repositories\CommentRepository;

class CommentController extends Controller
{
    /**
     * The CommentRepository instance.
     *
     * @var \App\Repositories\CommentRepository
     */
    protected $comment;

    /**
     * Create a new CommentController instance.
     *
     * @param \App\Repositories\CommentRepository $comment
     * @return void
     */
    public function __construct(CommentRepository $comment)
    {
        $this->comment = $comment;
    }

    /**
     * Store a newly created resource in storage.
     *
     * @param \App\Models\Post $post
     * @return
     */
    public function store(Post $post)
    {
        return $this->comment->create($post);
    }
}
