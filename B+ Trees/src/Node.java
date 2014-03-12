
public class Node {
	
	private String path;
	private String[] children;
	private long[] keys;
	private boolean isLeaf;
	private static final int fanOut = 2;

	public Node(String path, boolean isLeaf) {
		
		this.path = path;
		
		children = new String[fanOut];
		keys = new long[fanOut - 1];
		//hopefully the parts are null

		this.isLeaf = isLeaf;
	}
	
	private void readFake() {
		
		
	}
	
	/**
	 * Returns true if it needs to split. Too hard for now
	 * 
	 * If it doesn't need to, returns false
	 */
	public boolean addChild(String dataFile, long key) {
		
		if(isLeaf) {
			if(children[0] == null) {
				children[0] = dataFile;
			} else if(children[1] == null) {
				keys[0] = key;
				children[1] = dataFile;
			} else {			
				return true;
			}
		}
		
		writeChanges();
		
		return false;
	}
	
	private void writeChanges() {
		
		
	}
	
}
