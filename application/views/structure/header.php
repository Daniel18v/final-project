<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<div class="container-fluid">
    <div class="row">
        <div class="col-6-lg">
            <a href="<?= site_url() ?>">
                <img src="/images/header.png" class="img-responsive pull-left" alt="Cabecera"/>

            </a>
        </div>

        <?php if (!$sess->session->userdata('user')) :?>
        <div class="col-6-lg">
            <p>
                <button type="button" class="btn btn-sm btn-info pull-right login"><span class="glyphicon glyphicon-log-in"></span>  Iniciar sesión</button>

                <button type="button" class="margin-right-5 btn btn-sm btn-danger pull-right signup"><span class="glyphicon glyphicon-new-window"></span> Registrarse</button>
            </p>
        </div>
        <?php endif ?>
    </div>
</div>