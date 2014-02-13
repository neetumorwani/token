<?php

/**
 * @file
 * Contains \Drupal\token\Tests\TokenEntityTestCase.
 */
namespace Drupal\token\Tests;

/**
 * Tests entity tokens.
 */
class TokenEntityTestCase extends TokenTestBase {
  public static function getInfo() {
    return array(
      'name' => 'Entity token tests',
      'description' => 'Test the entity tokens.',
      'group' => 'Token',
    );
  }

  public function setUp($modules = array()) {
    $modules[] = 'taxonomy';
    parent::setUp($modules);

    // Create the default tags vocabulary.
    $vocabulary = entity_create('taxonomy_vocabulary', array(
      'name' => 'Tags',
      'machine_name' => 'tags',
    ));
    taxonomy_vocabulary_save($vocabulary);
    $this->vocab = $vocabulary;
  }

  function testEntityMapping() {
    $this->assertIdentical(token_get_entity_mapping('token', 'node'), 'node');
    $this->assertIdentical(token_get_entity_mapping('token', 'term'), 'taxonomy_term');
    $this->assertIdentical(token_get_entity_mapping('token', 'vocabulary'), 'taxonomy_vocabulary');
    $this->assertIdentical(token_get_entity_mapping('token', 'invalid'), FALSE);
    $this->assertIdentical(token_get_entity_mapping('entity', 'node'), 'node');
    $this->assertIdentical(token_get_entity_mapping('entity', 'taxonomy_term'), 'term');
    $this->assertIdentical(token_get_entity_mapping('entity', 'taxonomy_vocabulary'), 'vocabulary');
    $this->assertIdentical(token_get_entity_mapping('entity', 'invalid'), FALSE);

    // Test that when we send the mis-matched entity type into token_replace()
    // that we still get the tokens replaced.
    $vocabulary = taxonomy_vocabulary_machine_name_load('tags');
    $term = $this->addTerm($vocabulary);
    $this->assertIdentical(token_replace('[vocabulary:name]', array('taxonomy_vocabulary' => $vocabulary)), $vocabulary->name);
    $this->assertIdentical(token_replace('[term:name][term:vocabulary:name]', array('taxonomy_term' => $term)), $term->name . $vocabulary->name);
  }

  function addTerm(TaxonomyVocabulary $vocabulary, array $term = array()) {
    $term += array(
      'name' => drupal_strtolower($this->randomName(5)),
      'vid' => $vocabulary->vid,
    );
    $term = entity_create('taxonomy_term', $term);
    taxonomy_term_save($term);
    return $term;
  }

  /**
   * Test the [entity:original:*] tokens.
   */
  function testEntityOriginal() {
    $node = $this->drupalCreateNode(array('title' => 'Original title'));

    $tokens = array(
      'nid' => $node->nid,
      'title' => 'Original title',
      'original' => NULL,
      'original:nid' => NULL,
    );
    $this->assertTokens('node', array('node' => $node), $tokens);

    // Emulate the original entity property that would be available from
    // node_save() and change the title for the node.
    $node->original = entity_load_unchanged('node', $node->nid);
    $node->title = 'New title';

    $tokens = array(
      'nid' => $node->nid,
      'title' => 'New title',
      'original' => 'Original title',
      'original:nid' => $node->nid,
    );
    $this->assertTokens('node', array('node' => $node), $tokens);
  }
}