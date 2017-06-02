<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

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

            <li <?= ($_SERVER['REQUEST_URI'] == "/") ? "class='active'" : "" ?> ><a href="/"><i
                            class="glyphicon glyphicon-home"></i></a></li>
            <li <?= (strpos($_SERVER['REQUEST_URI'], "/tienda") !== false) ? "class='active'" : "" ?> ><a
                        href="/tienda">Tienda</a></li>
            <li <?= (strpos($_SERVER['REQUEST_URI'], "/juegos") !== false) ? "class='active'" : "" ?> ><a
                        href="/juegos">Juegos</a></li>
            <li <?= ($_SERVER['REQUEST_URI'] == "/noticias") ? "class='active'" : "" ?> ><a
                        href="/noticias">Noticias</a></li>
        </ul>

        <ul class="nav navbar-nav navbar-right">
            <?php if (isset($_SESSION['rol'])) : ?>
                <?php if (($_SESSION['rol'] == 1 OR $_SESSION['rol'] == 3)) : ?>
                    <li class="dropdown">
                        <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                            <span class='glyphicon glyphicon-user'></span> <?= $_SESSION['user'] ?>
                            <span class="caret"></span>
                        </a>
                        <ul class="dropdown-menu right">
                            <?= $_SESSION['rol'] == 1 ? "<li><a href=".site_url()."admin><span class='glyphicon glyphicon-wrench'></span> Administrador</a></li>" : "" ?>
                            <li><a href='#'><span class='glyphicon glyphicon-shopping-cart'></span> Carrito</a></li>
                            <li><a href="<?= site_url()."salir" ?>"><span class='glyphicon glyphicon glyphicon-log-out'></span> Salir</a></li>

                        </ul>
                    </li>
                <?php endif ?>
            <?php endif ?>

        </ul>
        <form class="navbar-form navbar-right" role="search">
            <div class="form-group input-group">
                <input type="text" class="form-control" placeholder="Buscar...">
                <span class="input-group-btn">
            <button class="btn btn-info" type="button">
              <span class="glyphicon glyphicon-search"></span>
            </button>
          </span>
            </div>
        </form>
    </div><!--/.nav-collapse -->
</div>