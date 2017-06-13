<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Blog_model extends CI_Model
{
    public function __construct() {
        $this->load->database();
    }

    public function insert_commentary($data, $slug) {
        $this->db->select('id');
        $query = $this->db->get_where('blog', array('slug' => '/blog/' . $slug));
        $data['id_publication'] = $query->result()[0]->id;
        $this->db->insert('comments', $data);
        echo 200;

    }

    public  function get_comments($slug) {
        $this->db->select('*');
        $this->db->where(array('slug' => '/blog/' . $slug));
        $this->db->from('blog');
        $this->db->join('comments', 'comments.id_publication = blog.id');
        $query = $this->db->get();
        return $query->result();
    }

    public function get_blog($slug){
        $query = $this->db->get_where('blog', array('slug' => '/blog/' . $slug));
        return $query->result();
    }

    public function get_blogs(){
        $this->db->order_by('id', 'DESC');
        $query = $this->db->get("blog");
        return $query->result_array();
    }
    public function insertData($data)
    {
        $this->db->insert('blog', $data);
    }

    public function deleteData($id)
    {
        $this->db->where('id', $id);
        $this->db->delete('blog');
    }

    public function updateData($data)
    {
        $this->db->where('id', $data['id']);
        $this->db->update('blog', $data);
    }
}