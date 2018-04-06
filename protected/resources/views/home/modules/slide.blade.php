<!-- View stored in resources/views/home/index.blade.php -->

<li data-transition="fade">
    <img src="{{ asset('images/slides/' . $slider->image) }}"
         alt="{{ $slider->alt }}"
         data-bgposition="center center"
         data-bgfit="cover"
         data-bgrepeat="no-repeat"
         class="rev-slidebg">

    <div class="tp-caption text-color-light custom-secondary-font"
         data-x="center" data-hoffset="0"
         data-y="center" data-voffset="-40"
         data-start="800"
         data-whitespace="nowrap"
         data-transform_in="y:[100%];s:500;"
         data-transform_out="opacity:0;s:500;"
         style="font-size: 22px;">{{ $slider->alt }}</div>

    <h1 class="tp-caption text-color-light custom-secondary-font font-weight-bold"
        data-x="center" data-hoffset="0"
        data-y="center" data-voffset="0"
        data-start="1100"
        data-whitespace="nowrap"
        data-transform_in="y:[100%];s:500;"
        data-transform_out="opacity:0;s:500;"
        style="font-size: 54px;">{{ $slider->title }}</h1>

    <div class="tp-caption text-uppercase text-color-light custom-secondary-font"
         data-x="center" data-hoffset="0"
         data-y="center" data-voffset="50"
         data-start="1400"
         data-whitespace="nowrap"
         data-transform_in="y:[100%];s:500;"
         data-transform_out="opacity:0;s:500;"
         style="font-size: 22px;">{{ $slider->subtitle }}</div>

    <a href="{{ $slider->link }}"
       class="tp-caption btn btn-primary custom-btn-style-1 text-uppercase"
       data-x="center" data-hoffset="0"
       data-y="center" data-voffset="140"
       data-start="1700"
       data-whitespace="nowrap"
       data-transform_in="y:[100%];s:500;"
       data-transform_out="opacity:0;s:500;"
       data-fontsize="['16','16','16','24']">{{ $slider->button }}</a>
</li>