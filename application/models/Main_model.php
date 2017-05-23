<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of StatusModel
 *
 * @author Cheke
 */
class Main_model extends CI_Model {
    /*
     * Load database
     */

    public function __construct() {
        $this->load->database();
    }

    /*
     * Returns all rows. 
     */ 
    public function  get_rom($name_url, $type) {
        $query = $this->db->get_where('roms', array('name-url' => $name_url, 'type' => $type), 1);
        return $query->result_array();
    }
    public function get_all() {
        $query = $this->db->get("roms");
        return $query->result_array();
    }

}
