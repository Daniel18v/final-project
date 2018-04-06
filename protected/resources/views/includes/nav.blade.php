<div class="container-fluid">
    <div class="navbar-header ">
        <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target=".navbar-collapse">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
        </button>
    </div>
    <div class="navbar-collapse collapse">
        <ul class="nav navbar-nav">
            <li class="{{ Request::is('/') ? "active" : "" }}">
                <a href="/">
                    <i class="glyphicon glyphicon-home"></i>
                </a>
            </li>
            <li class="{{ Request::is('/') ? "active" : "" }}">
                <a href="/juegos">
                    Juegos
                </a>
            </li>
            <li class="{{ Request::is('/') ? "active" : "" }}">
                <a href="/noticias">
                    Noticias
                </a>
            </li>
        </ul>

        <ul class="nav navbar-nav navbar-right">
            @if (auth()->guard('web')->check() OR auth()->guard('admin')->check())
                <li>
                    <img src="{{ asset(auth()->user()->avatar) ?? "" }}" class="img-circle avatar">
                </li>
                <li id="coins">
                    <a>
                        <span>{{ auth()->user()->points ?? ""}}</span>
                        <span class='glyphicon glyphicon-piggy-bank'></span>
                    </a>
                </li>
            @endif
            <li class="dropdown">
                <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                    {{ auth()->user()->name ?? "" }}
                    <span class="caret"></span>
                </a>
                <ul class="dropdown-menu right">
                    @auth('admin')
                    <li>
                        <a href="{{ url('admin') }}">
                            <span class='glyphicon glyphicon-wrench'></span> Administrador
                        </a>
                    </li>
                    @endauth
                    <li>
                        <a href="{{ url('perfil')  }}">
                            <span class='glyphicon glyphicon-user'></span> Mi perfil
                        </a>
                    </li>
                    <li>
                        <a href="{{ route('logout')  }}">
                            <span class='glyphicon glyphicon glyphicon-log-out'></span> Salir
                        </a>
                    </li>
                </ul>
            </li>
        </ul>
    </div><!--/.nav-collapse -->
</div>