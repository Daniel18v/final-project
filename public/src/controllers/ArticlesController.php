<?php

/**
 * Description of articlesController
 *
 * @author Cheke
 */

namespace FreeBird\controllers;

use FreeBird\models\ArticlesModel;

class ArticlesController {

    public function index() {
        require_once "src/models/ArticlesModel.php";
        $model = new ArticlesModel();
        $articles = $model->getArticles();
        require_once "/../../views/articlesView.php";
    }
    
    public function sayHello() {
        echo "Hola Sutanito";
    }

}
