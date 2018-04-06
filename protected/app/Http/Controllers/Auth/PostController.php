<?php

namespace App\Http\Controllers;

use App\Models\Post;
use App\Repositories\PostRepository;

class PostController extends Controller
{
    /**
     * The PostRepository instance.
     *
     * @var \App\Repositories\PostRepository
     */
    protected $post;

    /**
     * Create a new PostController instance.
     *
     * @param \App\Repositories\PostRepository $post
     * @return void
     */
    public function __construct(PostRepository $post)
    {
        $this->post = $post;

        $this->middleware('auth');
    }

    /**
     * Display a listing of the resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function index()
    {
        return view('blog.index');
    }

    /**
     * Show the form for creating a new resource.
     *
     * @return \Illuminate\Http\Response
     */
    public function create()
    {
        return view('blog.create');
    }

    /**
     * Store a newly created resource in storage.
     *
     * @return \Illuminate\Http\Response
     */
    public function store()
    {
        auth()->user()->publish($this->post->create(request()->all()));

        return redirect(route('blog'))->with('post-ok', __('The post has been succesfully created'));
    }

    /**
     * Display the specified resource.
     *
     * @param  \App\Models\Post $post
     * @return \Illuminate\Http\Response
     */
    public function show(Post $post)
    {
        return view('blog.show', compact('post'));
    }
}
