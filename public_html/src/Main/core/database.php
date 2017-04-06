<?php

/**
 * Description of database
 *
 * @author Cheke
 */
require_once __DIR__ . "/config.php";

namespace Main\core;

class DB {

    private $db;

    function __construct() {
        $this->db = new mysqli(DB_HOST, DB_USER, DB_PASS, DB_NAME);
        if ($this->db->connect_errno) {
            echo "Fail to connect to MySQL:" . $this->db->connect_errno;
            return;
        }
        $this->db->set_charset(DB_CHARSET);
    }

    public function consult($sql) {
        $rs = $this->db->query($sql);
        if (!$rs) {
            echo "MySQL Error: " . $this->db->connect_errno;
        }
        return $rs;
    }

    public function close() {
        $this->db->close();
    }

    public function __destruct() {
        
    }

}
