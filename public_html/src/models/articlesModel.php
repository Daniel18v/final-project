<?php

require_once "/../config/database.php";
require_once "/../config/constants.php";

class articlesModel {
    private $db;
    private $articles;
    
    function __construct() {
        $this->db = new DB();
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
