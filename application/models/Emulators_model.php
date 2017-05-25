<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Emulators_model extends CI_Model {

    public function __construct() {
        $this->load->database();
    }

    public function get_rom($type, $game) {
        $query = $this->db->select('title')
                ->where('title', $game)
                ->where( 'type',$type)
                ->get('roms');
        return $query->result();
    }
    public function get_roms($type) {
        $query = $this->db->select('roms')
                ->where('type', $type)
                ->get('roms');
        return $query->result_array();
    }



}