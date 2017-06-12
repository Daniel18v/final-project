<?php

defined("BASEPATH") or exit("No direct script access allowed");

class ACP_model extends CI_Model
{
    public function __construct() {
        $this->load->database();
    }

    public function get_roms()
    {
        $query = $this->db->get("roms");
        return $query->result_array();
    }

    public function insertData($data)
    {
        $this->db->insert('roms', $data);
    }

    public function deleteData($id)
    {
        $this->db->where('id', $id);
        $this->db->delete('roms');
    }

    public function updateData($data)
    {
        $this->db->where('id', $data['id']);
        $this->db->update('roms', $data);
    }
}