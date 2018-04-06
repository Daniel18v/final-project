<div class="container-fluid">
    <div class="row">
        <div class="col-6-lg">
            <a href="{{ route('/') }}">
                <img src="/images/header.png" class="img-responsive pull-left" alt="Cabecera"/>

            </a>
        </div>

        @guest
        <div class="col-6-lg">
            <p>
                <button type="button" class="btn btn-sm btn-info pull-right login">
                    <span class="glyphicon glyphicon-log-in"></span>  Iniciar sesión
                </button>
                <button type="button" class="margin-right-5 btn btn-sm btn-danger pull-right signup">
                    <span class="glyphicon glyphicon-new-window"></span> Registrarse
                </button>
            </p>
        </div>
        @endguest
    </div>
</div>