<div class="flash-message">
    @foreach (['danger', 'warning', 'success', 'info'] as $msg)
        @if(session()->has('alert-' . $msg))
            <p class="alert text-center alert-{{ $msg }}">
                {{ session()->get('alert-' . $msg) }}
                <a href="#" class="close" data-dismiss="alert" aria-label="close">&times;</a>
            </p>
        @endif
    @endforeach
</div> <!-- end .flash-message -->