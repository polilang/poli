#[derive(Debug, Clone)]
pub enum Expression {
    Integer(i64),
    Float(f64),
    Text(String),
    Bool(bool),
    Call(Box<Expression>, Box<Vec<Expression>>),
}
