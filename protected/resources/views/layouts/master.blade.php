<!DOCTYPE html>
<html lang="{{ app()->getLocale() }}">
<head>
@include('includes.head')
</head>

<header class="header">
    @include('includes.header')
</header>
<nav class="navbar navbar-inverse ">
    @include('includes.nav')
</nav>
<body class="body">
<div class="container-fluid wrapper">
     @yield('content')
</div>
<div class="push"></div>
@include('includes.footer')
@include('includes.scripts')
@yield('scripts')
<div>
    @include('includes.login')
    @include('includes.signup')
</div>

</body>
</html>
