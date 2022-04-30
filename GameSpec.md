# ゲーム仕様

## 共通
* キャラクター（RoguelikeCharacter）
  * ターンにつき n 回行動を行う
    * 攻撃
    * 移動
    * ...

* 移動能力（GridMovementComponent）
  * グリッド単位で移動
  * 指定のマスに移動
    * 経路探索

* 装備能力（EquipmentComponent）
  * 規定数の装備を管理
  * 

* アイテム基底クラス（ItemBase）
  * アイテムクラス（Item）
  * 装備（Equipment）

## 全体
  * ターン管理（GameManager クラス）
    * ターンを進めるたびにマップ全体のキャラクターの行動を進める
      * 起点はプレイヤーの操作
        * 連続移動したときの挙動
      * 全キャラクターの参照を持つ