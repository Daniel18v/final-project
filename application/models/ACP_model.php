<?php

defined("BASEPATH") or exit("No direct script access allowed");

class ACP_model extends CI_Model {

    public function __construct() {
        $this->load->database();
    }

    public function insertData($title, $description, $type, $rom, $images) {
        $data = array('title' => '$title', 'description' => '$description', 'type' => '$type', 'rom' => '$rom', 'images' => '$images');
        $this->db->insert('roms', $data);
    }

    public function deleteData($id) {
        $this->db->where('id', $id);
        $this->db->delete('roms');
    }

    public function updateData($id, $title, $description, $type, $rom, $images) {
        $data = array('id' => '$id', 'title' => '$title', 'description' => '$description', 'type' => '$type', 'rom' => '$rom', 'images' => '$images');
        $this->db->where('id', $id);
        $this->db->update('roms', $data);
    }

    public function selectData($data) {
        $this->db->where("type", "$data");
        $query = $this->db->get("roms");

        if ($query->num_rows() > 0) {
            return $query->result_array();
        }
    }
}