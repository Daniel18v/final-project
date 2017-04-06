<?php

namespace FreeBird\models; 

use FreeBird\core\Database;

require_once "/../../config/constants.php";

class ArticlesModel {
    private $db;
    private $articles;
    
    function __construct() {
        $this->db = new Database();
        $this->articles = array();
    }
    
    public function getArticles() {
        $sql = "SELECT * FROM " . TABLE_NAME;
        $rs = $this->db->consult($sql);
        while ($rows = $rs->fetch_assoc()) {
            $this->articles[] = $rows;
        }
        return $this->articles;
    }

}
