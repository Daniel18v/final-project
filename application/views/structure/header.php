<div class="container-fluid">
    <div class="row">
        <div class="col-6-md col-xs-4">
            <a href="<?= site_url() ?>">
                <img src="/images/header.png" class="img-responsive pull-left" alt="Cabecera"/>

            </a>
        </div>

        <? if (!isset($_SESSION['user'])) :?>
        <div class="col-6-md col-xs-8">
            <p>
                <button id="login" type="button" class="btn btn-sm btn-info pull-right "><span class="glyphicon glyphicon-log-in"></span>  Iniciar sesión</button>

                <button id="signin" type="button" class="margin-right-5 btn btn-sm btn-danger pull-right"><span class="glyphicon glyphicon-new-window"></span> Registrarse</button>
            </p>
        </div>
        <? endif ?>
    </div>
</div>
