<?php $this->layout("templates::master") ?>

<?php $this->start('header') ?>
<?php $this->insert("structure::header") ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>

    <div class="hr-margin-bottom-10"></div>

    <div class="row">

        <div class="col-lg-6 col-lg-offset-3">
            <div class="panel panel-info">
                <div class="panel-heading text-center">
                    <h3 class="panel-title">Administrador</h3>
                    <ul class="list-inline nav-bar">
                        <li><a href="/addRom">Añadir rom</a></li>
                        <li><a href="/deleteRom">Eliminar rom</a></li>
                        <li><a href="/updateRom">Modificar rom</a></li>
                        <li class="dropdown">
                            <a href="/listroms" class="dropdown-toggle" data-toggle="dropdown" role="button"
                               aria-haspopup="true"
                               aria-expanded="false">Listar Roms <span class="caret"></span></a>
                            <ul class="dropdown-menu">
                                <li><a href="/#" id="Nintendo">Nintendo</a></li>
                                <li><a href="/#" id="Master System">Master System</a></li>
                            </ul>
                        </li>
                    </ul>
                </div>
            </div>
        </div>

        <div class="col-lg-6 col-lg-offset-3">
            <div class="panel panel-info">
                <div class="panel-heading text-center">
                    <h3 class="panel-title"></h3>
                    <ul class="list-inline">
                    </ul>
                </div>
            </div>
        </div>

    </div>

<?php $this->stop() ?>