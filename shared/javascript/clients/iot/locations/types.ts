export interface APILocationType {
  id: number;
  name: string;
  tags: string[];
}

export const isAPILocationType = (o: any): o is APILocationType =>
  o.id != null && o.name != null && o.tags != null;
