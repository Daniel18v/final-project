<section class="section section-no-border background-color-light m-none">
    <div class="container">
        <div class="row">
            <div class="col-md-12 col-sm-8 col-md-offset-0 col-sm-offset-2">
                <h2 class="font-weight-bold">Blog Post</h2>
            </div>
        </div>
        <div class="row">
            @foreach(Post::latestByNumber() as $post)
                @include('blog.modules.post')
            @endforeach
        </div>
    </div>
</section>