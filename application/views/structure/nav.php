<div class="container-fluid">
    <div class="navbar-header">
        <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target=".navbar-collapse">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
        </button>
    </div>
    <div class="navbar-collapse collapse">
        <ul class="nav navbar-nav">
            <li class="active"><a href="/"><i class="glyphicon glyphicon-home"></i></a></li>
            <li><a href="#about">Tienda</a></li>
            <li><a href="#contact">Juegos</a></li>
            <li class="dropdown">
                <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-haspopup="true"
                   aria-expanded="false">Social <span class="caret"></span></a>
                <ul class="dropdown-menu">
                    <li><a href="#">Noticias</a></li>
                    <li><a href="#">Blog</a></li>
                    <li><a href="#">Foro</a></li>
                </ul>

            </li>
        </ul>

        <ul class="nav navbar-nav navbar-right">
            <?= (isset($_SESSION)) ? "" :
                    "<li><a href=\"#\"><span class=\"glyphicon glyphicon-user\"></span> Mi cuenta</a></li>" ?>
            <?= (isset($_SESSION)) ? "" :
                    "<li><a href=\"#\"><span class=\"glyphicon glyphicon-shopping-cart\"></span> Carrito</a></li>" ?>
        </ul>
        <form class="navbar-form navbar-right" role="search">
            <div class="form-group input-group">
                <input type="text" class="form-control" placeholder="Buscar..">
                <span class="input-group-btn">
            <button class="btn btn-info" type="button">
              <span class="glyphicon glyphicon-search"></span>
            </button>
          </span>
            </div>
        </form>
    </div><!--/.nav-collapse -->
</div>