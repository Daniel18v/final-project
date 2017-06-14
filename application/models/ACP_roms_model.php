<?php

defined("BASEPATH") or exit("No direct script access allowed");

class ACP_roms_model extends CI_Model {
    public function __construct() {
        $this->load->database();
    }

    public function get_roms() {
        $query = $this->db->get("roms");

        return $query->result_array();
    }

    public function insertRom($data) {
        $this->db->insert('roms', $data);
    }

    public function deleteRom($data) {
        $this->db->where('id', $data['id']);
        $this->db->delete('roms');
    }

    public function updateRom($data) {
        $this->db->where('id', $data['id']);
        $this->db->update('roms', $data);
    }
}