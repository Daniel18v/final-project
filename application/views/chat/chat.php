<div class="embed-responsive embed-responsive-4by3">
    <?php if(isset($_SESSION['user'])): ?>
        <iframe width="560" height="900" src="http://freebird.ddns.net:3030/?user=<?=$_SESSION['user']?>" frameborder="0" allowfullscreen></iframe>
    <?php else: ?>
        <label>Inicia sesión para poder hablar en el chat</label>
        <button type="button" class="btn btn-sm btn-info pull-right login"><span class="glyphicon glyphicon-log-in"></span>  Iniciar sesión</button>
    <?php endif ?>
</div>