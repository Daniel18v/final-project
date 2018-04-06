@extends($layout)

@section('content')
    <div role="main" class="main">

        <div class="slider-container rev_slider_wrapper" style="height: 600px;">
            <div id="revolutionSlider" class="slider rev_slider manual">
                <ul>

                </ul>
            </div>
        </div>

        @include('home.modules.callToAction')
        {{--@include('home.modules.blog')--}}
    </div>
@stop