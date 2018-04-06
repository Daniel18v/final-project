<div role="main" class="main">
    <section>
        @if (isset($anime->header))
            @if (file_exists(public_path() . "/images/headers/{$anime->header}"))
                <img class="mb-xlg img-responsive" src="{{ asset("images/headers/{$anime->header}") }}">
            @else
                <img class="mb-xlg img-responsive" src="{{ $anime->header }}">
            @endif
        @else
            <img class="mb-xlg img-responsive" src="{{ asset("images/headers/header.png") }}">
        @endif
    </section>
</div>