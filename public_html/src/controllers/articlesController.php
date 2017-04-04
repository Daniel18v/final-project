<?php

/**
 * Description of articlesController
 *
 * @author Cheke
 */
class articlesController {

    public function index() {
        require_once "src/models/articlesModel.php";
        $model = new articlesModel();
        $articles = $model->getArticles();
        require_once "src/views/articlesView.php";
    }
    
    public function sayHello() {
        echo "Hola Sutanito";
    }

}
